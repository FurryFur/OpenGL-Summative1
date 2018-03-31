#version 450 core

in VertexData {
	vec3 normal;
	vec2 texCoord;
	vec3 viewDir;
} i;

layout (std140) uniform ShaderParams {
	float metallicness;
	float glossiness;
} p;

out vec4 outColor;

uniform sampler2D sampler;
uniform samplerCube environmentSampler;

const float PI = 3.1415926535897932384626433832795;
const vec3 lightDir = vec3(0.5, 1, 1);
const vec3 LiDirect = vec3(2, 2, 2);
const vec3 LiAmbient = vec3(0.2, 0.2, 0.2);
const float kDiffNorm = 1 / PI;

void main(void)
{
	vec3 normal;
	if (gl_FrontFacing)
		normal = normalize(i.normal);
	else
		normal = -normalize(i.normal);

	vec3 color = texture(sampler, i.texCoord).rgb;

	// Direct Lighting variables
	vec3 viewDir = normalize(i.viewDir);
	vec3 halfVector = normalize(normalize(lightDir) + viewDir);
	float ndotl = clamp(dot(normalize(lightDir), normal), 0, 1);
	float ndoth = clamp(dot(normal, halfVector), 0, 1);

	// Reflection variables
	// vec3 LiReflDir = reflect(-viewDir, normal);
	// float ndotRl = clamp(dot(normalize(LiReflDir), normal), 0, 1);
	// vec3 LiReflHalfVec = normalize(normalize(LiReflDir) + viewDir);
	// float ndotRh = clamp(dot(normal, LiReflHalfVec), 0, 1);
	
	//vec3 LiRefl = texture(environmentSampler, LiReflDir).rgb;
	
	float specPow = p.glossiness;
	float specNorm = (specPow + 4) * (specPow + 2) / (8 * PI * (specPow + pow(2, -specPow / 2)));
	vec3 BRDFdiff = (1 - p.metallicness) * kDiffNorm * color;
	vec3 BRDFspec = p.metallicness * specNorm * color * pow(ndoth, specPow);
	// vec3 BRDFrefl = p.metallicness * specNorm * color; // n dot hrefl is always 1
	vec3 BRDFdirect = BRDFdiff + BRDFspec;

	//vec3 LrRefl = LiRefl * BRDFrefl * ndotRl;
	vec3 LrDirect = LiDirect * BRDFdirect * ndotl;
	vec3 LrAmbient = color * LiAmbient;

	outColor = vec4(LrDirect + LrAmbient, 1);

	if (outColor.r < 0.1f)
		discard;
}

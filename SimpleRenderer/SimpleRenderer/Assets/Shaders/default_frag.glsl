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

//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : stegu
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//               https://github.com/stegu/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

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
	vec3 LiReflDir = normalize(reflect(-viewDir, normal));
	vec3 LiReflBiTangent = normalize(cross(LiReflDir, -viewDir));
	vec3 LiReflTangent = normalize(cross(LiReflDir, LiReflBiTangent));
	float rnd = snoise(i.texCoord);
	float rnd2 = snoise(i.texCoord + 1);
	LiReflDir = normalize(LiReflDir + 1 / p.glossiness * (rnd * LiReflTangent + rnd2 * LiReflBiTangent));
	vec3 LiRefl = texture(environmentSampler, LiReflDir).rgb;
	vec3 LiReflHalfVec = normalize(LiReflDir + viewDir);
	float ndotRl = clamp(dot(LiReflDir, normal), 0, 1);
	float ndotRh = clamp(dot(normal, LiReflHalfVec), 0, 1);
	
	float specPow = p.glossiness;
	float specNorm = (specPow + 4) * (specPow + 2) / (8 * PI * (specPow + pow(2, -specPow / 2)));
	vec3 BRDFdiff = (1 - p.metallicness) *  kDiffNorm * color;
	vec3 BRDFspec = p.metallicness * specNorm * color * pow(ndoth, specPow);
	vec3 BRDFrefl = p.metallicness * specNorm * color * pow(ndotRh, specPow);
	vec3 BRDFdirect = BRDFdiff + BRDFspec;

	vec3 LrRefl = LiRefl * BRDFrefl * ndotRl;
	vec3 LrDirect = LiDirect * BRDFdirect * ndotl;
	vec3 LrAmbient = color * LiAmbient;

	outColor = vec4(LrRefl + LrAmbient + LrDirect, 1);
}

#version 450 core

in VertexData {
	vec3 normal;
	vec2 texCoord;
	vec3 viewDir;
} i;

out vec4 outColor;

void main(void)
{
	outColor = vec4(0.4, 0.4, 1.0, 1);

	vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	outColor = mix(outColor, fogColor, pow(gl_FragCoord.z, 50.0));
}

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
}

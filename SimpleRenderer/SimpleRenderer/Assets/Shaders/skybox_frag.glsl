#version 450 core

in VertexData {
    vec3 textureDir;
} i;

out vec4 outColor;

uniform samplerCube skybox;

void main(void)
{
    outColor = texture(skybox, i.textureDir);

	vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	outColor = mix(outColor, fogColor, pow(gl_FragCoord.z, 50.0));
}

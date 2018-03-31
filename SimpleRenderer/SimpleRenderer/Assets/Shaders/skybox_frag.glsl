#version 450 core

in VertexData {
    vec3 textureDir;
} i;

out vec4 outColor;

uniform samplerCube skybox;

void main(void)
{
    outColor = texture(skybox, i.textureDir);
}

#version 450 core

layout (location = 0) in vec3 inPosition;

layout (std140) uniform Uniforms {
    mat4 model;
    mat4 view;
    mat4 projection;
	vec4 cameraPos;
} u;

out VertexData {
    vec3 textureDir;
} o;

void main()
{
	mat4 view = u.view;
	view[3] = vec4(0, 0, 0, 1);
	o.textureDir = inPosition;
	gl_Position = (u.projection * view * vec4(inPosition, 1.0)).xyww;
}

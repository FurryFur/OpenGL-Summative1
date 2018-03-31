#version 450 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

layout (std140) uniform Uniforms {
    mat4 model;
    mat4 view;
    mat4 projection;
	vec4 cameraPos;
} u;

out VertexData {
    vec3 normal;
    vec2 texCoord;
	vec3 viewDir;
} o;

void main()
{
	vec3 worldPos = (u.model * vec4(inPosition, 1)).xyz;

    o.normal = (u.model * vec4(inNormal, 0)).xyz; // TODO: Do inverse transpose
    o.texCoord = inTexCoord;
	o.viewDir = (u.cameraPos.xyz - worldPos).xyz;

    gl_Position = u.projection * u.view * vec4(worldPos, 1);
}

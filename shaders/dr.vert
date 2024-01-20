#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(push_constant) uniform PushConstant
{
    mat4 projection;
    mat4 view;
} pc;

layout(location = 0) out vec3 fragColor;

void main()
{
    fragColor = color;
    gl_Position = pc.projection * pc.view * vec4(position, 1.0);
}
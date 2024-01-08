#version 460
#extension GL_EXT_shader_8bit_storage : require
#extension GL_GOOGLE_include_directive : require

#include "mesh.h"

vec3 rotateQuat(vec3 v, vec4 q)
{
    return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

layout(push_constant) uniform PushConstant
{
    mat4 projection;
    mat4 view;
} pc;

layout(set = 0, binding = 0) readonly buffer VertexBuffer
{
    Vertex vertices[];
};

layout(std140, set = 0, binding = 1) readonly buffer ObjectBuffer
{
    ObjectData objects[];
};

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec3 fragPos;
layout(location = 2) out vec3 fragLightPos;
layout(location = 3) out uint fragMaterialId;

void main()
{
    vec3 position = vec3(vertices[gl_VertexIndex].vx, vertices[gl_VertexIndex].vy, vertices[gl_VertexIndex].vz);
    vec3 normal = vec3(int(vertices[gl_VertexIndex].nx), int(vertices[gl_VertexIndex].ny), int(vertices[gl_VertexIndex].nz)) / 127.0 - 1.0;
    vec2 texcoord = unpackUnorm2x16(vertices[gl_VertexIndex].uv);
    fragNormal = mat3(transpose(inverse(pc.view * objects[gl_InstanceIndex].model))) * normal;
    fragPos = vec3(pc.view * objects[gl_InstanceIndex].model * vec4(position, 1));
    fragLightPos = vec3(pc.view * vec4(0.0, 2.0, 0.0, 1.0));
    fragMaterialId = objects[gl_InstanceIndex].materialIndex;
    gl_Position = pc.projection * pc.view * objects[gl_InstanceIndex].model * vec4(position, 1);
}
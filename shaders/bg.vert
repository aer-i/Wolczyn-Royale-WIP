#version 460
#extension GL_EXT_shader_16bit_storage : require
#extension GL_EXT_shader_8bit_storage : require

struct Vertex
{
    float vx, vy, vz;
    uint8_t nx, ny, nz, nw;
    float16_t tu, tv;
};

struct MeshDraw
{
    vec3 position;
    float scale;
    vec4 orientation;
};

struct ObjectData
{
    mat4 model;
};

vec3 rotateQuat(vec3 v, vec4 q)
{
    return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

layout(push_constant) uniform PushConstant
{
    mat4 projectionView;
} pc;

layout(set = 0,binding = 0) readonly buffer VertexBuffer
{
    Vertex vertices[];
};

layout(std140, set = 0, binding = 1) readonly buffer ObjectBuffer
{
    ObjectData objects[];
} ob;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec3 fragPos;

void main()
{
    vec3 position = vec3(vertices[gl_VertexIndex].vx, vertices[gl_VertexIndex].vy, vertices[gl_VertexIndex].vz);
    vec3 normal = vec3(int(vertices[gl_VertexIndex].nx), int(vertices[gl_VertexIndex].ny), int(vertices[gl_VertexIndex].nz)) / 127.0 - 1.0;
    vec2 texcoord = vec2(vertices[gl_VertexIndex].tu, vertices[gl_VertexIndex].tv);
    fragNormal = mat3(transpose(inverse(ob.objects[gl_BaseInstance].model))) * normal;
    fragPos = vec3(ob.objects[gl_BaseInstance].model * vec4(position, 1));
    gl_Position = pc.projectionView * ob.objects[gl_BaseInstance].model * vec4(position, 1);
}
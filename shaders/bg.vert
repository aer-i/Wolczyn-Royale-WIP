#version 460
#extension GL_EXT_buffer_reference : require
#extension GL_EXT_shader_16bit_storage : require
#extension GL_EXT_shader_8bit_storage : require

struct Vertex
{
    float vx, vy, vz;
    uint8_t nx, ny, nz, nw;
    float16_t tu, tv;
};

layout(push_constant) uniform PushConstant
{
    mat4 projectionView;
} pc;

layout(binding = 0) readonly buffer VertexBuffer
{
    Vertex vertices[];
};

layout(location = 0) out vec3 fragNormal;

void main()
{
    vec3 position = vec3(vertices[gl_VertexIndex].vx, vertices[gl_VertexIndex].vy, vertices[gl_VertexIndex].vz);
    vec3 normal = vec3(int(vertices[gl_VertexIndex].nx), int(vertices[gl_VertexIndex].ny), int(vertices[gl_VertexIndex].nz)) / 127.0 - 1.0;
    vec2 texcoord = vec2(vertices[gl_VertexIndex].tu, vertices[gl_VertexIndex].tv);
    fragNormal = normal;
    gl_Position = pc.projectionView * vec4(position, 1);
}
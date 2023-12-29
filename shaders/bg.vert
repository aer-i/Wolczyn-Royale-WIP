#version 460
#extension GL_EXT_buffer_reference : require

struct Vertex
{
    vec3 pos;
    float uvX;
    vec3 col;
    float uvY;
};

layout(buffer_reference, std430) readonly buffer VertexBuffer{
    Vertex vertices[];
};

layout(push_constant) uniform PushConstant
{
    VertexBuffer vertexBuffer;
} pc;

void main()
{
    Vertex v = pc.vertexBuffer.vertices[gl_VertexIndex];

    gl_Position = vec4(v.pos, 1);
}
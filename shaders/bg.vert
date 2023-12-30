#version 460
#extension GL_EXT_buffer_reference : require

struct Vertex
{
    vec3 pos;
    float uvX;
    vec3 normal;
    float uvY;
};

layout(buffer_reference, std430) readonly buffer VertexBuffer{
    Vertex vertices[];
};

layout(push_constant) uniform PushConstant
{
    mat4 projectionView;
    VertexBuffer vertexBuffer;
} pc;

layout(location = 0) out vec3 fragNormal;

void main()
{
    Vertex v = pc.vertexBuffer.vertices[gl_VertexIndex];
    fragNormal = v.normal;
    gl_Position = pc.projectionView * vec4(v.pos, 1);
}
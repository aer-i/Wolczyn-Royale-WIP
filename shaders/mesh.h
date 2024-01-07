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
    uint materialIndex;
    uint padding1;
    uint padding2;
    uint padding3;
};
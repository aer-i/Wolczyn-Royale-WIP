struct Vertex
{
    float vx, vy, vz;
    uint8_t nx, ny, nz, nw;
    float u, v;
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
};
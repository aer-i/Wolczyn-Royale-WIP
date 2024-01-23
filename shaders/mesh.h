struct Vertex
{
    float vx, vy, vz;
    uint8_t nx, ny, nz, nw;
    float u, v;
};

struct ObjectData
{
    mat4 model;
    uint materialIndex;
};
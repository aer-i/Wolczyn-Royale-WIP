#pragma once
#include <Arln.hpp>

struct Vertex
{
    arln::f32 vx, vy, vz;
    arln::u8  nx, ny, nz, nw;
    arln::u32 uv;
};

struct Mesh {
    explicit Mesh(std::string_view t_fp, std::vector<Vertex>& t_vb, std::vector<arln::u32>& t_ib) noexcept;
    Mesh() = default;
    arln::u32 ic{};
    arln::i32 vxo{};
    arln::u32 ixo{};
};

struct Model
{
    Mesh* msh{ nullptr };
    arln::mat4 mtx{};
};
#pragma once
#include <Arln.hpp>

struct Vertex
{
    arln::f32 vx, vy, vz;
    arln::u8  nx, ny, nz, nw;
    arln::u16 tu, tv;
};

struct Mesh {
    explicit Mesh(std::string_view t_fp, std::vector<Vertex>& t_vb, std::vector<arln::u32>& t_ib) noexcept;
    Mesh() = default;
    arln::u32 ic{};
    arln::i32 vxo{};
};

struct Model
{
    std::reference_wrapper<Mesh> msh;
    arln::mat4 mtx{};
};
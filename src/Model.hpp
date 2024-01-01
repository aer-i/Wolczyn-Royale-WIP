#pragma once
#include <Arln.hpp>

struct Vertex
{
    arln::f32 vx, vy, vz;
    arln::u8  nx, ny, nz, nw;
    arln::u16 tu, tv;
};

struct Mesh {
    explicit Mesh(std::string_view t_fp) noexcept;
    Mesh() = default;
    arln::Buffer vb, ib;
    arln::u32 ic{};
};

struct Material
{
    arln::Pipeline p;
    arln::Descriptor d;
};

struct Model
{
    std::reference_wrapper<Mesh> msh;
    std::reference_wrapper<Material> mtr;
    arln::mat4 mtx;
};
#pragma once
#include "Camera.hpp"

struct Vertex
{
    arln::f32 vx, vy, vz;
    arln::u8  nx, ny, nz, nw;
    arln::u16 tu, tv;
};

struct Mesh {
    arln::Pipeline mts;
    arln::Buffer vb, ib;
    arln::Descriptor ds;
    arln::u32 ic;
    arln::mat4 matrix;
};

class Scene
{
    using v0 = void;
public:
    Scene(arln::Window& t_w, arln::Context& t_c) noexcept;
    ~Scene() noexcept;
    inline auto& gMhs() noexcept {
        return m_mhs;
    } inline auto& gCm() noexcept {
        return m_cm;
    }
    auto lm(std::vector<Vertex> const& t_v, std::vector<arln::u32> const& t_i) noexcept -> v0;
    auto lm(std::string_view t_pth) noexcept -> v0;
    auto u() noexcept -> v0;
private:
    arln::Context& m_ctx;
    arln::DescriptorPool m_dp;
    arln::Buffer m_ob;
    Camera m_cm;
    std::vector<Mesh> m_mhs;
};
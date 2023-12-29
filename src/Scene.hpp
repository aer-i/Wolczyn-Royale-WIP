#pragma once
#include "Camera.hpp"

struct Vertex
{
    arln::vec3 pos;
    arln::f32  uvX;
    arln::vec3 col;
    arln::f32  uvY;
};

struct Mesh {
    arln::Pipeline mts;
    arln::Buffer vb, ib;
};

class Scene
{
    using v0 = void;
public:
    struct PD {
        arln::mat4 pv;
        arln::u64 dp;
    };
public:
    Scene(arln::Window& t_w, arln::Context& t_c) noexcept;
    ~Scene() noexcept;
    inline auto& gMhs() noexcept {
        return m_mhs;
    } inline auto& gCm() noexcept {
        return m_cm;
    }
    auto lm(std::vector<Vertex> const& t_v, std::vector<arln::u32> const& t_i) noexcept -> v0;
    auto u() noexcept -> v0;
private:
    arln::Context& m_ctx;
    Camera m_cm;
    std::vector<Mesh> m_mhs;
};
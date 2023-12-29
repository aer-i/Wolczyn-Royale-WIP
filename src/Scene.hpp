#pragma once
#include <Arln.hpp>

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
    explicit Scene(arln::Context& t_c) noexcept;
    ~Scene() noexcept;
    inline auto& gMhs() noexcept {
        return m_mhs;
    }
    auto lm(std::vector<Vertex> const& t_v, std::vector<arln::u32> const& t_i) noexcept -> v0;
private:
    arln::Context& m_ctx;
    std::vector<Mesh> m_mhs;
};
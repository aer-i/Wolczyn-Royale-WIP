#pragma once
#include <Arln.hpp>

struct Vertex
{
    arln::f32 vx, vy, vz;
    arln::u8  nx, ny, nz, nw;
    arln::f32 u, v;
};

struct Mesh {
    arln::u32 ic{};
    arln::i32 vxo{};
    arln::u32 ixo{};
    glm::vec3 ctr{};
    arln::f32 rad{};
};

struct Model
{
    std::vector<Mesh> mhs;
    arln::u32 mtID{};
    glm::vec3 pos{};
    glm::vec3 rot{};
    glm::vec3 scl{1.f};

    [[nodiscard]] inline auto gMtx() const noexcept -> glm::mat4 {
        return glm::translate(glm::mat4{1.f}, pos) * glm::eulerAngleXYX(rot.x, rot.y, rot.z) * glm::scale(glm::mat4{1.f}, scl);
    }
};

struct Material {
    arln::u32 dt, st;
    arln::f32 shin;
};
#pragma once
#include <Arln.hpp>
#include <reactphysics3d/reactphysics3d.h>

struct Vertex
{
    arln::f32 vx, vy, vz;
    arln::u8  nx, ny, nz, nw;
    arln::f32 u, v;
};

struct Mesh {
    explicit Mesh(std::string_view t_fp, std::vector<Vertex>& t_vb, std::vector<arln::u32>& t_ib) noexcept;
    Mesh() = default;
    arln::u32 ic{};
    arln::i32 vxo{};
    arln::u32 ixo{};
    arln::vec3 ctr{};
    arln::f32 rad{};
};

struct Model
{
    Mesh* msh{ nullptr };
    arln::u32 txID{};
    arln::u32 mtID{};
    arln::vec3 pos{};
    arln::vec3 rot{};
    arln::vec3 scl{1.f};
    rp3d::RigidBody* rb{ nullptr };
    rp3d::Collider* col{ nullptr };

    [[nodiscard]] inline auto gMtx() const noexcept -> arln::mat4 {
        return glm::translate(arln::mat4{1.f}, pos) * glm::eulerAngleXYX(rot.x, rot.y, rot.z) * glm::scale(arln::mat4{1.f}, scl);
    }
};

struct Material {
    arln::u32 dt, st;
    arln::f32 shin;
};
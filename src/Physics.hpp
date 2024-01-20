#pragma once
#include <reactphysics3d/reactphysics3d.h>

class Physics
{
    using v0 = void;
public:
    Physics() noexcept;
    ~Physics() noexcept;
    auto u() noexcept -> v0;
    static auto cScs(float t_rad) noexcept -> rp3d::SphereShape*;
    static auto cBcs(rp3d::Vector3 const& t_s) noexcept -> rp3d::BoxShape*;
    static auto cCcs(float t_rad, float t_hei) noexcept -> rp3d::CapsuleShape*;
    inline auto* gdr() noexcept {
        return m_dr;
    } inline auto cRB(rp3d::Transform const& t_tr) noexcept {
        return m_pw->createRigidBody(t_tr);
    }
private:
    rp3d::PhysicsWorld* m_pw{ nullptr };
    rp3d::DebugRenderer* m_dr{ nullptr };
};
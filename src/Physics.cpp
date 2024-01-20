#include "Physics.hpp"
#include "Time.hpp"

static rp3d::PhysicsCommon g_pc;

Physics::Physics() noexcept
{
    m_pw = g_pc.createPhysicsWorld();
    m_pw->setIsDebugRenderingEnabled(true);
    m_dr = std::addressof(m_pw->getDebugRenderer());
    m_dr->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
    m_dr->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    m_dr->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
    m_dr->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
}

Physics::~Physics() noexcept
{
    g_pc.destroyPhysicsWorld(m_pw);
}

auto Physics::u() noexcept -> v0
{
    static double accumulator = 0;
    static double previousFrameTime = 0;
    constexpr double timeStep = 1.0f / 60.0f;

    accumulator += Time::gDt();

    while (accumulator >= timeStep) {

        m_pw->update(timeStep);

        accumulator -= timeStep;
    }
}

auto Physics::cScs(float t_rad) noexcept -> rp3d::SphereShape* {
    return g_pc.createSphereShape(t_rad);
}

auto Physics::cBcs(rp3d::Vector3 const& t_s) noexcept -> rp3d::BoxShape*
{
    return g_pc.createBoxShape(t_s);
}

auto Physics::cCcs(float t_rad, float t_hei) noexcept -> rp3d::CapsuleShape*
{
    return g_pc.createCapsuleShape(t_rad, t_hei);
}

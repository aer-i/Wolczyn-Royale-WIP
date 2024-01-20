#include "Physics.hpp"
#include "Time.hpp"

Physics::Physics() noexcept
{
}

Physics::~Physics() noexcept
{
}

auto Physics::u() noexcept -> v0
{
    static double accumulator = 0;
    static double previousFrameTime = 0;
    constexpr double timeStep = 1.0f / 60.0f;

    accumulator += Time::gDt();

    while (accumulator >= timeStep) {


        accumulator -= timeStep;
    }
}
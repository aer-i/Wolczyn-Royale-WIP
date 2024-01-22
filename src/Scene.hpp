#pragma once
#include "SceneLoader.hpp"

class Scene : public SceneLoader {
public:
    inline explicit Scene(arln::Window& t_wn) noexcept : cam{ t_wn, {3, 3, 3}, 90.f } {}
    auto u() noexcept -> v0;
    auto pmd() noexcept -> v0;
    auto pr() noexcept -> v0;
public:
    Camera cam;
};
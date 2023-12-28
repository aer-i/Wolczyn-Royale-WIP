#pragma once
#include <Arln.hpp>
#include "Renderer.hpp"

class Engine {
    using v0 = void;
public:
    Engine() noexcept;
    auto exe() noexcept -> v0;
private:
    arln::Window m_wn;
    arln::Context m_cx;
    Renderer m_rn;
};
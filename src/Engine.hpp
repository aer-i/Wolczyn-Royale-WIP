#pragma once
#include <Arln.hpp>

class Engine
{
public:
    using v0 = void;
public:
    Engine() noexcept;
    auto exe() noexcept -> v0;

private:
    arln::Window m_wn;
    arln::Context m_cx;
};
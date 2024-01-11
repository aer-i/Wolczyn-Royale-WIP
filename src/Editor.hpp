#pragma once
#include "Arln.hpp"

class Editor
{
    using v0 = void;
public:
    Editor(arln::Window& t_w, arln::Context& t_c) noexcept;
    ~Editor() noexcept;
    auto e() noexcept -> v0;
    auto r(arln::CommandBuffer& t_cmd) noexcept -> v0;
private:
    arln::Window& m_wnd;
    arln::Context& m_ctx;
};
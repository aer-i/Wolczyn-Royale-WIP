#pragma once
#include "Arln.hpp"

class Editor
{
    using v0 = void;
public:
    Editor(arln::Window& t_w, arln::Context& t_c) noexcept;
    ~Editor() noexcept;
    auto e() noexcept -> v0;
    auto r() noexcept -> v0;
    [[nodiscard]] inline auto& gc() const noexcept {
        return m_cmd;
    }
private:
    arln::Window& m_wnd;
    arln::Context& m_ctx;
    arln::CommandBuffer m_cmd;
};
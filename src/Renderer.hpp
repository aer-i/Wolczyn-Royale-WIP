#pragma once
#include <Arln.hpp>
#include "Editor.hpp"

class Renderer {
    using v0 = void;
public:
    Renderer() = delete;
    Renderer(arln::Window& t_w, arln::Context& t_c) noexcept;
    ~Renderer() = default;
    auto drF() noexcept -> v0;
private:
    arln::Window& m_wnd;
    arln::Context& m_ctx;
    Editor m_ed;
    arln::CommandBuffer m_cmd;
    std::vector<arln::CommandBufferHandle> m_cS;
};
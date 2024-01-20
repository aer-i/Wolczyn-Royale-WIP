#pragma once
#include "Editor.hpp"
#include "Scene.hpp"
#include "ThreadPool.hpp"

class Renderer {
    using v0 = void;
public:
    Renderer() = delete;
    Renderer(arln::Window& t_w, arln::Context& t_c) noexcept;
    ~Renderer() noexcept;
    auto drF(Scene& t_rnS) noexcept -> v0;
private:
    arln::Window& m_wnd;
    arln::Context& m_ctx;
    arln::CommandBuffer m_cmd{}, m_gCmd;
    arln::Image m_cAtt, m_dAtt;
    arln::Buffer m_drb, m_drbl;
    arln::Pipeline m_drp, m_drpl;
    Editor m_ed;
    ThreadPool m_tp{ 2 };
};
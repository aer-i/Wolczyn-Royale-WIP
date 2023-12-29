#include "Engine.hpp"
#include <iostream>

static void infC(std::string_view t_i) { std::cout << t_i << std::endl; }
static void errC(std::string_view t_e) { std::cerr << t_e << std::endl; }

static constexpr const char* g_t = "Light Frame";
static constexpr unsigned g_sw = 1280, g_sh = 720;

Engine::Engine() noexcept
    : m_wn{
        arln::WindowCreateInfo{
            .size = { g_sw, g_sh },
            .title = g_t,
            .errorCallback = errC
        }
    }, m_cx{
        arln::ContextCreateInfo{
            .errorCallback = errC,
            .infoCallback = infC,
            .getWindowWidthFunc = [&]{ return m_wn.getWidth(); },
            .getWindowHeightFunc = [&]{ return m_wn.getHeight(); },
            .surfaceCreation = [&](VkInstance t_i){ return m_wn.createSurface(t_i); },
            .extensions = m_wn.getInstanceExtensions(),
#ifndef _NDEBUG
            .layers = { "VK_LAYER_KHRONOS_validation" },
#endif
            .applicationName = g_t,
            .engineName = g_t,
        }
    }, m_rn{ m_wn, m_cx }
{}

auto Engine::exe() noexcept -> v0 {
    while (not m_wn.shouldClose()) {
        m_wn.pollEvents();
        if (m_cx.canRender()) {
            m_rn.drF(); }
    }
}

#include "Engine.hpp"
#include "Time.hpp"
#include <iostream>

static constinit unsigned g_cs = 0;

static void infC(std::string_view t_i) { std::cout << t_i << std::endl; }
static void errC(std::string_view t_e) { std::cerr << t_e << std::endl; SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", t_e.data(), nullptr); std::cin.get(); }

static constexpr const char* g_t = "Wolczyn Royale";
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
            .errorCallback = errC, .infoCallback = infC,
            .getWindowWidthFunc = [&]{ return m_wn.getWidth(); }, .getWindowHeightFunc = [&]{ return m_wn.getHeight(); },
            .surfaceCreation = [&](VkInstance t_i){ return m_wn.createSurface(t_i); }, .extensions = m_wn.getInstanceExtensions(),
#ifndef NDEBUG
            .layers = { "VK_LAYER_KHRONOS_validation" },
#endif
            .applicationName = g_t, .engineName = g_t,
            .presentMode = arln::PresentMode::eNoSync
        }
    }, m_rn{ m_wn, m_cx }
{ m_ss.emplace_back(m_wn, m_cx);}

auto Engine::exe() noexcept -> v0 {
    while (!m_wn.shouldClose()) {
        m_wn.pollEvents();
        Time::u();
        m_ss[g_cs].u();
        if (m_cx.canRender()) {
            m_rn.drF(m_ss[g_cs]); }
    }
}

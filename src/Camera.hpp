#pragma once
#include <Arln.hpp>

class Camera
{
    using v0 = void;
public:
    explicit Camera(arln::Window& t_wnd) noexcept : m_wn{ t_wnd } {}
    auto u() noexcept -> v0;
    auto sP(arln::f32 t_f, arln::f32 t_ar, arln::f32 t_ne = 0.1f, arln::f32 t_fa = 256.f) noexcept -> v0;
    auto sV(arln::vec3 const& t_di) noexcept -> v0;
    inline auto sVT(arln::vec3 const& t_t) noexcept -> v0 {
        sV(t_t - m_ps);
    } [[nodiscard]]inline auto gPV() const noexcept -> arln::mat4 {
        return m_pr * m_vi;
    }
private:
    arln::Window& m_wn;
    arln::mat4 m_pr{}, m_vi{};
    arln::vec3 m_fr{}, m_up{},
       m_ps{}, m_rt{}, m_ri{};
    arln::f32 m_ya{}, m_pi{};
};
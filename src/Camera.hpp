#pragma once
#include <Arln.hpp>

class Camera
{
    using v0 = void;
public:
    explicit Camera(arln::Window& t_wnd, arln::vec3 const& t_sps = {}, arln::f32 t_y = {}, arln::f32 t_p = {}) noexcept
    : m_wn{ t_wnd }, m_ps{ t_sps }, m_ya{ t_y }, m_pi{ t_p } {}
    auto u() noexcept -> v0;
    auto sP(arln::f32 t_f, arln::f32 t_ar, arln::f32 t_ne = 0.1f, arln::f32 t_fa = 256.f) noexcept -> v0;
    auto sV(arln::vec3 const& t_di) noexcept -> v0;
    inline auto sVT(arln::vec3 const& t_t) noexcept -> v0 {
        sV(t_t - m_ps);
    } [[nodiscard]]inline auto gPi() const noexcept -> arln::f32 {
        return m_pi;
    } [[nodiscard]]inline auto gPs() const noexcept -> arln::vec3 const& {
        return m_ps;
    } [[nodiscard]]inline auto gFr() const noexcept -> arln::vec3 const& {
        return m_fr;
    } [[nodiscard]]inline auto gYa() const noexcept -> arln::f32 {
        return m_ya;
    } [[nodiscard]]inline auto gP() const noexcept -> arln::mat4 const& {
        return m_pr;
    } [[nodiscard]]inline auto gV() const noexcept -> arln::mat4 const& {
        return m_vi;
    } [[nodiscard]]inline auto gRt() const noexcept -> arln::vec3 const {
        return glm::vec3{ std::cos(arln::toRadians(m_ya)) * std::cos(arln::toRadians(m_pi)),
                          std::sin(arln::toRadians(m_pi)),
                          std::sin(arln::toRadians(m_ya)) * std::cos(arln::toRadians(m_pi))};
    }
private:
    arln::Window& m_wn;
    arln::mat4 m_pr{}, m_vi{};
    arln::vec3 m_fr{}, m_up{},
               m_ps{}, m_rt{};
    arln::f32  m_ya{}, m_pi{};
};
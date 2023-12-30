#include "Camera.hpp"
#include <cmath>

auto Camera::u() noexcept -> v0 {
    arln::f32 dt = .006f;

    if (ImGui::IsKeyDown(ImGuiKey_W)) {
        m_ps += m_fr * dt * 5.f;
    } if (ImGui::IsKeyDown(ImGuiKey_S)) {
        m_ps -= m_fr * dt * 5.f;
    } if (ImGui::IsKeyDown(ImGuiKey_D)) {
        m_ps += m_rt * dt * 5.f;
    } if (ImGui::IsKeyDown(ImGuiKey_A)) {
        m_ps -= m_rt * dt * 5.f;
    } if (ImGui::IsKeyDown(ImGuiKey_E)) {
        m_ps.y += dt * 5.f;
    } if (ImGui::IsKeyDown(ImGuiKey_Q)) {
        m_ps.y -= dt * 5.f;
    }

    m_ya = glm::mod(m_ya + 0.01f, 360.f);

    if (m_pi > 89.9f) m_pi = 89.9f;
    if (m_pi < -89.9f) m_pi = -89.9f;

    arln::f32 x = std::cos(arln::toRadians(m_ya)) * std::cos(arln::toRadians(m_pi)),
              y = std::sin(arln::toRadians(m_pi)),
              z = std::sin(arln::toRadians(m_ya)) * std::cos(arln::toRadians(m_pi));

    m_fr = normalize(glm::vec3{x, y, z});
    m_rt = normalize(cross(m_fr, { 0.f, 1.f, 0.f }));
    m_up = normalize(cross(m_rt, m_fr));

    sV(m_fr);
}

auto Camera::sP(arln::f32 t_f, arln::f32 t_ar, arln::f32 t_ne, arln::f32 t_fa) noexcept -> v0 {
    arln::f32 const thf = std::tan(t_f / 2.f);
    m_pr = glm::mat4{ 0.0f };
    m_pr[0][0] = 1.f / (t_ar * thf);
    m_pr[1][1] = -(1.f / (thf));
    m_pr[2][2] = -(t_fa + t_ne) / (t_fa - t_ne);
    m_pr[2][3] = -1.f;
    m_pr[3][2] = -(2 * t_fa * t_ne) / (t_fa - t_ne);
}

auto Camera::sV(const arln::vec3& t_di) noexcept -> v0 {
    auto const w{ normalize(t_di) },
               u{ normalize(cross(w, { 0.f, 1.f, 0.f })) },
               v{ cross(u, w) };
    m_vi = glm::mat4(1.f);
    m_vi[0][0] = u.x;
    m_vi[1][0] = u.y;
    m_vi[2][0] = u.z;
    m_vi[0][1] = v.x;
    m_vi[1][1] = v.y;
    m_vi[2][1] = v.z;
    m_vi[0][2] = -w.x;
    m_vi[1][2] = -w.y;
    m_vi[2][2] = -w.z;
    m_vi[3][0] = -dot(u, m_ps);
    m_vi[3][1] = -dot(v, m_ps);
    m_vi[3][2] = dot(w, m_ps);
}
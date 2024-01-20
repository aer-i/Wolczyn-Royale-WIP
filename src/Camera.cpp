#include "Camera.hpp"
#include "Time.hpp"
#include <cmath>

auto Camera::u() noexcept -> v0 {
    arln::f32 dt = Time::gDt();
    static arln::f32 sn = 0.1f;
    auto wkp = ImGui::GetMainViewport()->WorkPos;
    ImGui::SetNextWindowPos({ wkp.x + 2, wkp.y + 70 }, ImGuiCond_Always);
    if (ImGui::Begin("Mouse settings", nullptr,
             ImGuiWindowFlags_NoDecoration |
             ImGuiWindowFlags_AlwaysAutoResize |
             ImGuiWindowFlags_NoBackground |
             ImGuiWindowFlags_NoMove |
             ImGuiWindowFlags_NoNav |
             ImGuiWindowFlags_NoFocusOnAppearing |
             ImGuiWindowFlags_NoSavedSettings |
             ImGuiWindowFlags_NoDocking
         )) {
        ImGui::SliderFloat("Mouse sens", &sn, 0.01f, 10.f, "%.2f");
    } ImGui::End();

    glm::vec3 vel{};
    arln::f32 sp = 5.f;
    static bool rmm = false;

    if (m_wn.getKeyDown(arln::Key::eEscape)) {
        rmm = !rmm;
        m_wn.setRelativeMouseMode(rmm);
    } if (m_wn.getKey(arln::Key::eW)) {
        vel -= m_fr;
    } if (m_wn.getKey(arln::Key::eS)) {
        vel += m_fr;
    } if (m_wn.getKey(arln::Key::eD)) {
        vel += m_rt;
    } if (m_wn.getKey(arln::Key::eA)) {
        vel -= m_rt;
    } if (m_wn.getKey(arln::Key::eE)) {
        vel.y += 1.f;
    } if (m_wn.getKey(arln::Key::eQ)) {
        vel.y -= 1.f;
    } if (m_wn.getKey(arln::Key::eLeftShift)) {
        sp += 5.f;
    }

    if (vel != arln::vec3{0.f}) {
        m_ps += normalize(vel) * dt * sp;
    }

    if (rmm) {
        m_ya = glm::mod(m_ya - m_wn.getCursorOffsetX() * sn, 360.f);
        m_pi += m_wn.getCursorOffsetY() * sn;
    }

    if (m_pi > 89.9f) m_pi = 89.9f;
    if (m_pi < -89.9f) m_pi = -89.9f;

    m_fr = normalize(this->gRt());
    m_rt = normalize(cross(m_fr, { 0.f, 1.f, 0.f }));
    m_up = normalize(cross(m_rt, m_fr));

    sV(m_fr);
}

auto Camera::sP(arln::f32 t_f, arln::f32 t_ar, arln::f32 t_ne, arln::f32 t_fa) noexcept -> v0 {
    arln::f32 const thf = std::tan(t_f / 2.f);
    m_pr = glm::mat4{ 0.0f };
    m_pr[0][0] = 1.f / (t_ar * thf);
    m_pr[1][1] = 1.f / (thf);
    m_pr[2][2] = t_fa / (t_fa - t_ne);
    m_pr[2][3] = 1.f;
    m_pr[3][2] = -(t_fa * t_ne) / (t_fa - t_ne);
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
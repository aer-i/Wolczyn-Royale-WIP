#pragma once
#include "Camera.hpp"
#include "Model.hpp"
#include "Sky.hpp"
#include <unordered_map>

class Scene
{
    using v0 = void;
public:
    Scene(arln::Window& t_w, arln::Context& t_c) noexcept;
    ~Scene() noexcept;
    [[nodiscard]]inline auto& gIb() noexcept {
        return m_ib;
    } [[nodiscard]]inline auto& gMls() noexcept {
        return m_mls;
    } [[nodiscard]]inline auto& gCm() noexcept {
        return m_cm;
    } [[nodiscard]]inline auto& gGp() noexcept {
        return m_gp;
    } [[nodiscard]]inline auto& gDs() noexcept {
        return m_ds;
    } [[nodiscard]]inline auto& gSky() noexcept {
        return m_sky;
    }
    auto u() noexcept -> v0;
    auto lMdl(std::string_view t_msh, std::string_view t_mtr, arln::vec3 const& t_pos) noexcept -> v0;
    auto lMhs(std::string_view t_n, std::string_view t_fp) noexcept -> v0;
private:
    auto pr() noexcept -> v0;
    auto pms() noexcept -> v0;
    auto pmd() noexcept -> v0;

private:
    arln::Context& m_ctx;
    arln::DescriptorPool m_dp;
    arln::Descriptor m_ds;
    arln::Buffer m_ob, m_vb, m_ib;
    arln::Pipeline m_gp;
    Camera m_cm;
    Sky m_sky;
    std::vector<Model> m_mls;
    std::unordered_map<std::string, Mesh> m_mhs;
    std::vector<Vertex> m_vv;
    std::vector<arln::u32> m_iv;
};
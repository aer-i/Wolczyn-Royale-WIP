#pragma once
#include "Camera.hpp"
#include "Model.hpp"
#include "Sky.hpp"
#include "Frustum.hpp"
#include <unordered_map>

class Scene
{
    using v0 = void;
public:
    Scene(arln::Window& t_w, arln::Context& t_c) noexcept;
    ~Scene() noexcept;
    [[nodiscard]]inline auto& gIb() noexcept {
        return m_ib;
    } [[nodiscard]]inline auto& gIdb() noexcept {
        return m_idb;
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
    } [[nodiscard]]inline auto gDc() const noexcept {
        return m_dc;
    }
    auto u() noexcept -> v0;
    auto lMdl(std::string_view t_msh, std::string_view t_mtr, arln::vec3 const& t_pos = {},
              arln::vec3 const& t_rot = {}, arln::vec3 const& t_scl = arln::vec3{1.f}) noexcept -> v0;
    auto lMhs(std::string_view t_n, std::string_view t_fp) noexcept -> v0;
    auto lMtr(std::string_view t_n, arln::vec3 const& t_a, arln::vec3 const& t_d, arln::vec3 const& t_s, arln::f32 t_shn) noexcept -> v0;
private:
    auto pr() noexcept -> v0;
    auto pms() noexcept -> v0;
    auto pmd() noexcept -> v0;
    auto gidb() noexcept -> v0;

private:
    arln::Context& m_ctx;
    arln::DescriptorPool m_dp;
    arln::Descriptor m_ds;
    arln::Buffer m_ob, m_mb, m_vb, m_ib, m_idb;
    arln::Pipeline m_gp;
    arln::u32 m_dc;
    Camera m_cm;
    Sky m_sky;
    Frustum m_fru;
    std::vector<Model> m_mls;
    std::vector<Material> m_mtHls;
    std::unordered_map<std::string, Mesh> m_mhs;
    std::unordered_map<std::string, arln::u32> m_mts;
    std::vector<Vertex> m_vv;
    std::vector<arln::u32> m_iv;
};
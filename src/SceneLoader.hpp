#pragma once
#include "Camera.hpp"
#include "Model.hpp"
#include "Sky.hpp"
#include "Physics.hpp"
#include <unordered_map>

struct SceneLoader {
protected:
    using v0 = void;
public:
    SceneLoader() noexcept {}
    ~SceneLoader() noexcept;
protected:
    auto lMdl(std::string_view t_msh, std::string_view t_mtr, arln::vec3 const& t_pos = {},
              arln::vec3 const& t_rot = {}, arln::vec3 const& t_scl = arln::vec3{1.f}) noexcept -> v0;
    auto lMhs(std::string_view t_n, std::string_view t_fp) noexcept -> v0;
    auto lMtr(std::string_view t_n, std::string_view t_tx, arln::f32 t_shn) noexcept -> v0;
    auto lTxt(std::string_view t_n, std::string_view t_fp) noexcept -> v0;
    auto gIdb() noexcept -> v0;
public:
    arln::DescriptorPool m_dp;
    arln::Descriptor m_ds{};
    arln::Buffer m_ob, m_mb, m_vb, m_ib, m_idb;
    arln::Pipeline m_gp{};
    arln::Sampler m_sm;
    arln::u32 m_dc{};
    Sky m_sky;
    Physics m_phx;
    std::vector<Model> m_mls;
    std::vector<Material> m_mtHls;
    std::vector<arln::Image> m_txHls;
    std::unordered_map<std::string, Mesh> m_mhs;
    std::unordered_map<std::string, arln::u32> m_mts;
    std::unordered_map<std::string, arln::u32> m_txs;
    std::vector<Vertex> m_vv;
    std::vector<arln::u32> m_iv;
};
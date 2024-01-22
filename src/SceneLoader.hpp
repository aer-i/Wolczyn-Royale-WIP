#pragma once
#include "Camera.hpp"
#include "Model.hpp"
#include "Sky.hpp"
#include "Physics.hpp"
#include "MeshImporter.hpp"
#include <unordered_map>

struct SceneLoader {
protected:
    using v0 = void;
public:
    SceneLoader() = default;
    ~SceneLoader() noexcept;
protected:
    auto lMdl(std::string_view t_fp, glm::vec3 const& t_pos = {}, glm::vec3 const& t_rot = {}, glm::vec3 const& t_scl = glm::vec3{1.f}) noexcept -> v0;
    auto gIdb() noexcept -> v0;
public:
    arln::DescriptorPool m_dp;
    arln::Descriptor m_ds{};
    arln::Buffer m_ob, m_idb;
    arln::Pipeline m_gp{};
    arln::u32 m_dc{};
    Sky m_sky;
    Physics m_phx;
    MeshImporter m_mshImp;
    std::vector<Model> m_mls;
};
#pragma once
#include "Camera.hpp"
#include "Model.hpp"
#include <unordered_map>

class Scene
{
    using v0 = void;
public:
    Scene(arln::Window& t_w, arln::Context& t_c) noexcept;
    ~Scene() noexcept;
    inline auto& gMls() noexcept {
        return m_mls;
    } inline auto& gCm() noexcept {
        return m_cm;
    }
    auto u() noexcept -> v0;
    auto lMdl(std::string_view t_msh, std::string_view t_mtr) noexcept -> v0;
    auto lMhs(std::string_view t_n, std::string_view t_fp) noexcept -> v0;
    auto lMtr(std::string_view t_n, arln::GraphicsPipelineInfo&& t_pi) noexcept -> v0;
private:
    arln::Context& m_ctx;
    arln::DescriptorPool m_dp;
    arln::Buffer m_ob;
    Camera m_cm;
    std::vector<Model> m_mls;
    std::unordered_map<std::string, Mesh> m_mhs;
    std::unordered_map<std::string, Material> m_mts;
};
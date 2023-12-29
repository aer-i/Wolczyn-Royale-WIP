#include "Scene.hpp"
#include <algorithm>

Scene::Scene(arln::Context& t_c) noexcept : m_ctx{ t_c } {
    auto pi = arln::GraphicsPipelineInfo{
        .vertShaderPath = "shaders/bg.vert.spv",
        .fragShaderPath = "shaders/bg.frag.spv"
    };
    m_mats.emplace_back(t_c.createGraphicsPipeline(pi));
}

Scene::~Scene() noexcept {
    std::for_each(m_mats.begin(), m_mats.end(), [](arln::Pipeline& m) {
        m.destroy();
    });
}

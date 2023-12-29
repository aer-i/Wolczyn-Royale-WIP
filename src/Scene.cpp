#include "Scene.hpp"
#include <algorithm>

Scene::Scene(arln::Context& t_c) noexcept : m_ctx{ t_c } {

}

Scene::~Scene() noexcept {
    std::for_each(m_mhs.begin(), m_mhs.end(), [](Mesh& m) {
        m.mts.destroy();
        m.vb.free();
        m.ib.free();
    });
}

auto Scene::lm(const std::vector<Vertex>& t_v, const std::vector<arln::u32>& t_i) noexcept -> v0 {
    auto pi = arln::GraphicsPipelineInfo{
        .vertShaderPath = "shaders/bg.vert.spv", .fragShaderPath = "shaders/bg.frag.spv"
    }; pi.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eVertex, 8, 0);
    auto vb = m_ctx.allocateBuffer(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpu, t_v.size() * sizeof(t_v[0]));
    vb.writeData(t_v.data(), vb.getSize());
    m_mhs.emplace_back(m_ctx.createGraphicsPipeline(pi), vb);
}
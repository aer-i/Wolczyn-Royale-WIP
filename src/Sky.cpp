#include "Sky.hpp"

Sky::Sky() noexcept
{
    auto pi = arln::GraphicsPipelineInfo{
        .vertShaderPath = "shaders/sky.vert.spv",
        .fragShaderPath = "shaders/sky.frag.spv",
        .cullMode = arln::CullMode::eNone,
        .topology = arln::Topology::eTriangleFan
    };
    pi.depthFormat = arln::CurrentContext()->getDefaultDepthFormat();
    pi.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eFragment, sizeof(PC), 0);

    m_p = arln::CurrentContext()->createGraphicsPipeline(pi);
}

Sky::~Sky() noexcept
{
    m_p.destroy();
}

auto Sky::r(arln::CommandBuffer& t_cmd, Camera& t_cm) noexcept -> v0
{
    auto e = arln::CurrentContext()->getCurrentExtent();
    auto p = PC{
        .mps = { arln::toRadians(t_cm.gPi()), arln::toRadians(t_cm.gYa()) },
        .res = e
    };

    t_cmd.bindGraphicsPipeline(m_p);
    t_cmd.setScissor(0, 0, e.x, e.y);
    t_cmd.setViewport(0, 0, arln::f32(e.x), arln::f32(e.y));
    t_cmd.pushConstant(m_p, arln::ShaderStageBits::eFragment, sizeof(PC), &p);
    t_cmd.draw(4);
}
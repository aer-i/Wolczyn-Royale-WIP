#include "Renderer.hpp"
#include "Time.hpp"

struct Drd {
    glm::vec3 pos;
    arln::u32 col;
};

Renderer::Renderer(arln::Window& t_w) noexcept : m_wnd{ t_w },
    m_ed{ t_w } {
    m_cmd = arln::CurrentContext()->allocateCommandBuffer();
    m_gCmd = arln::CurrentContext()->allocateCommandBuffer();
    arln::GraphicsPipelineInfo drpI{};
    drpI.vertShaderPath = "shaders/dr.vert.spv";
    drpI.fragShaderPath = "shaders/dr.frag.spv";
    drpI.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eVertex, sizeof(glm::mat4) * 2, 0);
    drpI.bindings << arln::BindingDescription(0, sizeof(Drd), arln::VertexInputRate::eVertex);
    drpI.attributes << arln::AttributeDescription(0, 0, arln::Format::eR32G32B32Sfloat, offsetof(Drd, pos))
                    << arln::AttributeDescription(1, 0, arln::Format::eR8G8B8Unorm, offsetof(Drd, col));
    drpI.depthFormat = arln::CurrentContext()->getDefaultDepthFormat();
    drpI.topology = arln::Topology::eTriangleList;
    drpI.polygonMode = arln::PolygonMode::eLine;
    drpI.depthStencil = true;
    m_drp = arln::CurrentContext()->createGraphicsPipeline(drpI);
    drpI.topology = arln::Topology::eLineList;
    drpI.polygonMode = arln::PolygonMode::eFill;
    m_drpl = arln::CurrentContext()->createGraphicsPipeline(drpI);
    m_cAtt.recreate(
        arln::CurrentContext()->getCurrentExtent().x,
        arln::CurrentContext()->getCurrentExtent().y,
        arln::CurrentContext()->getDefaultColorFormat(),
        arln::ImageUsageBits::eColorAttachment | arln::ImageUsageBits::eSampled,
        arln::MemoryType::eDedicated
    ); m_dAtt.recreate(
        arln::CurrentContext()->getCurrentExtent().x,
        arln::CurrentContext()->getCurrentExtent().y,
        arln::CurrentContext()->getDefaultDepthFormat(),
        arln::ImageUsageBits::eDepthStencilAttachment,
        arln::MemoryType::eDedicated
    ); arln::CurrentContext()->setResizeCallback([&](arln::u32 t_w, arln::u32 t_h){
        m_cAtt.recreate(
            t_w,
            t_h,
            arln::CurrentContext()->getDefaultColorFormat(),
            arln::ImageUsageBits::eColorAttachment,
            arln::MemoryType::eDedicated
        ); m_dAtt.recreate(
            t_w,
            t_h,
            arln::CurrentContext()->getDefaultDepthFormat(),
            arln::ImageUsageBits::eDepthStencilAttachment,
            arln::MemoryType::eDedicated
    );});
}

auto Renderer::drF(Scene& t_rnS) noexcept -> v0 {
    arln::CurrentContext()->beginFrame();
    {
        auto cAtt = arln::ColorAttachmentInfo {
            .clearColor = { .5f, .5f, .75f, 1.f },
            .image = arln::CurrentContext()->getPresentImage()
        }; auto dAtt = arln::DepthAttachmentInfo{
            .image = m_dAtt
        }; m_cmd.begin(); m_cmd.transitionImages({
            arln::ImageTransitionInfo{
                .image = arln::CurrentContext()->getPresentImage(),
                .oldLayout = arln::ImageLayout::eUndefined, .newLayout = arln::ImageLayout::eColorAttachment,
                .srcStageMask = arln::PipelineStageBits::eColorAttachmentOutput, .dstStageMask = arln::PipelineStageBits::eColorAttachmentOutput,
                .srcAccessMask = arln::AccessBits::eNone, .dstAccessMask = arln::AccessBits::eColorAttachmentWrite
            }, arln::ImageTransitionInfo{
                .image = m_dAtt,
                .oldLayout = arln::ImageLayout::eUndefined, .newLayout = arln::ImageLayout::eDepthAttachment,
                .srcStageMask = arln::PipelineStageBits::eLateFragmentTests | arln::PipelineStageBits::eEarlyFragmentTests,
                .dstStageMask = arln::PipelineStageBits::eLateFragmentTests | arln::PipelineStageBits::eEarlyFragmentTests,
                .srcAccessMask = arln::AccessBits::eNone, .dstAccessMask = arln::AccessBits::eDepthStencilAttachmentWrite
            }
        });
        m_cmd.beginRendering(arln::RenderingInfo{
            .pColorAttachment = &cAtt, .pDepthAttachment = &dAtt
        });
        //t_rnS.m_sky.r(m_cmd, t_rnS.cam);
        glm::mat4 pc[2] = {
            t_rnS.cam.gP(),
            t_rnS.cam.gV()
        }; m_cmd.bindGraphicsPipeline(t_rnS.m_gp);
        m_cmd.setScissor(0, 0, m_wnd.getWidth(), m_wnd.getHeight());
        m_cmd.setViewport(0, (arln::f32)m_wnd.getHeight(), (arln::f32)m_wnd.getWidth(), -(arln::f32)m_wnd.getHeight());
        m_cmd.pushConstant(t_rnS.m_gp, arln::ShaderStageBits::eVertex, sizeof(glm::mat4) * 2, pc);
        m_cmd.bindDescriptorGraphics(t_rnS.m_gp, t_rnS.m_ds);
        m_cmd.bindIndexBuffer32(t_rnS.m_mshImp.gIb());
        m_cmd.drawIndexedIndirect(t_rnS.m_idb, 0, t_rnS.m_dc, sizeof(arln::DrawIndexedIndirectCommand));
        m_cmd.endRendering();
//        m_cmd.transitionImages(arln::ImageTransitionInfo{
//            .image = arln::CurrentContext()->getPresentImage(),
//            .oldLayout = arln::ImageLayout::eColorAttachment, .newLayout = arln::ImageLayout::ePresentSrc,
//            .srcStageMask = arln::PipelineStageBits::eColorAttachmentOutput, .dstStageMask = arln::PipelineStageBits::eColorAttachmentOutput,
//            .srcAccessMask = arln::AccessBits::eColorAttachmentWrite, .dstAccessMask = arln::AccessBits::eNone
//        });
        //m_cmd.end();
    }

    m_ed.r(m_cmd);

    arln::CurrentContext()->endFrame({ m_cmd });
}

Renderer::~Renderer() noexcept {
    m_dAtt.free();
    m_cAtt.free();
    m_drp.destroy();
    m_drpl.destroy();
    m_drb.free();
    m_drbl.free();
}

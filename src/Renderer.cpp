#include "Renderer.hpp"

Renderer::Renderer(arln::Window& t_w, arln::Context& t_c) noexcept : m_wnd{ t_w }, m_ctx{ t_c },
    m_ed{ t_w, t_c } {
    m_cmd = m_ctx.allocateCommandBuffer();
    m_dAtt.recreate(
        m_ctx.getCurrentExtent().x,
        m_ctx.getCurrentExtent().y,
        m_ctx.getDefaultDepthFormat(),
        arln::ImageUsageBits::eDepthStencilAttachment,
        arln::MemoryType::eDedicated
    ); m_ctx.setResizeCallback([&](arln::u32 t_w, arln::u32 t_h){
        m_dAtt.recreate(
            t_w,
            t_h,
            m_ctx.getDefaultDepthFormat(),
            arln::ImageUsageBits::eDepthStencilAttachment,
            arln::MemoryType::eDedicated
    );});
}

auto Renderer::drF(Scene& t_rnS) noexcept -> v0 {
    m_ctx.beginFrame(); {
        auto cAtt = arln::ColorAttachmentInfo {
            .clearColor = { .5f, .5f, .75f, 1.f },
            .image = m_ctx.getPresentImage()
        }; auto dAtt = arln::DepthAttachmentInfo{
            .image = m_dAtt
        }; m_cmd.begin(); m_cmd.transitionImages({
            arln::ImageTransitionInfo{
                .image = m_ctx.getPresentImage(),
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
        t_rnS.gSky().r(m_cmd, t_rnS.gCm());
        Mesh* pmsh{ nullptr };
        auto pc = t_rnS.gCm().gPV();
        m_cmd.bindGraphicsPipeline(t_rnS.gGp());
        m_cmd.setScissor(0, 0, m_wnd.getWidth(), m_wnd.getHeight());
        m_cmd.setViewport(0, (arln::f32)m_wnd.getHeight(), (arln::f32)m_wnd.getWidth(), -(arln::f32)m_wnd.getHeight());
        m_cmd.pushConstant(t_rnS.gGp(), arln::ShaderStageBits::eVertex, sizeof(pc), &pc);
        m_cmd.bindDescriptorGraphics(t_rnS.gGp(), t_rnS.gDs());

        for (arln::u32 i = 0; auto& m : t_rnS.gMls()) {
            if (pmsh != m.msh) {
                pmsh = m.msh;
                m_cmd.bindIndexBuffer32(t_rnS.gIb(), pmsh->ixo);
            } m_cmd.drawIndexed(m.msh->ic, 1, 0, m.msh->vxo, i++);
        } m_cmd.endRendering();
        m_cmd.end();
    } m_ed.r();
    m_ctx.endFrame({ m_cmd, m_ed.gc() });
}

Renderer::~Renderer() noexcept {
    m_dAtt.free();
}

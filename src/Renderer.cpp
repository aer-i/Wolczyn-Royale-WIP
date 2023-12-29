#include "Renderer.hpp"

Renderer::Renderer(arln::Window& t_w, arln::Context& t_c) noexcept : m_wnd{ t_w }, m_ctx{ t_c },
    m_ed{ t_w, t_c } {
    m_cmd = m_ctx.allocateCommandBuffer();
}

auto Renderer::drF(Scene& t_rnS) noexcept -> v0 {
    m_ctx.beginFrame(); {
        auto cAtt = arln::ColorAttachmentInfo {
            .clearColor = { .75f, .25f, .5f, 1.f},
            .image = m_ctx.getPresentImage()
        }; m_cmd.begin(); m_cmd.transitionImages(arln::ImageTransitionInfo{
            .image = m_ctx.getPresentImage(),
            .oldLayout = arln::ImageLayout::eUndefined, .newLayout = arln::ImageLayout::eColorAttachment,
            .srcStageMask = arln::PipelineStageBits::eTopOfPipe, .dstStageMask = arln::PipelineStageBits::eColorAttachmentOutput,
            .srcAccessMask = arln::AccessBits::eNone, .dstAccessMask = arln::AccessBits::eColorAttachmentWrite
        });
        m_cmd.beginRendering(arln::RenderingInfo{
            .pColorAttachment = &cAtt
        });
        for (auto& m : t_rnS.gMt()) {
            m_cmd.bindGraphicsPipeline(m);
            m_cmd.setScissor(0, 0, m_wnd.getWidth(), m_wnd.getHeight());
            m_cmd.setViewport(0, (arln::f32)m_wnd.getHeight(), (arln::f32)m_wnd.getWidth(), -(arln::f32)m_wnd.getHeight());
            m_cmd.draw(3);
        } m_cmd.endRendering();
        m_cmd.end();
    } m_ed.r();
    m_ctx.endFrame({ m_cmd, m_ed.gc() });
}

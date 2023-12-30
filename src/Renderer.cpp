#include "Renderer.hpp"

Renderer::Renderer(arln::Window& t_w, arln::Context& t_c) noexcept : m_wnd{ t_w }, m_ctx{ t_c },
    m_ed{ t_w, t_c } {
    m_cmd = m_ctx.allocateCommandBuffer();
    m_dAtt.recreate(
        m_wnd.getWidth(),
        m_wnd.getHeight(),
        m_ctx.getDefaultDepthFormat(),
        arln::ImageUsageBits::eDepthStencilAttachment,
        arln::MemoryType::eGpuOnly
    );
}

auto Renderer::drF(Scene& t_rnS) noexcept -> v0 {
    m_ctx.beginFrame(); {
        auto cAtt = arln::ColorAttachmentInfo {
            .clearColor = { .75f, .25f, .5f, 1.f },
            .image = m_ctx.getPresentImage()
        }; auto dAtt = arln::DepthAttachmentInfo{
            .image = m_dAtt
        }; m_cmd.begin(); m_cmd.transitionImages({
            arln::ImageTransitionInfo{
                .image = m_ctx.getPresentImage(),
                .oldLayout = arln::ImageLayout::eUndefined, .newLayout = arln::ImageLayout::eColorAttachment,
                .srcStageMask = arln::PipelineStageBits::eTopOfPipe, .dstStageMask = arln::PipelineStageBits::eColorAttachmentOutput,
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
        for (auto& m : t_rnS.gMhs()) {
            Scene::PD pd = {
                .pv = t_rnS.gCm().gPV(),
                .dp = *m.vb.getDeviceAddress()
            }; m_cmd.bindGraphicsPipeline(m.mts);
            m_cmd.setScissor(0, 0, m_wnd.getWidth(), m_wnd.getHeight());
            m_cmd.setViewport(0, (arln::f32)m_wnd.getHeight(), (arln::f32)m_wnd.getWidth(), -(arln::f32)m_wnd.getHeight());
            m_cmd.pushConstant(m.mts, arln::ShaderStageBits::eVertex, sizeof(Scene::PD), &pd);
            m_cmd.bindIndexBuffer32(m.ib);
            m_cmd.drawIndexed(m.ic);
        } m_cmd.endRendering();
        m_cmd.end();
    } m_ed.r();
    m_ctx.endFrame({ m_cmd, m_ed.gc() });
}

Renderer::~Renderer() noexcept {
    m_dAtt.free();
}

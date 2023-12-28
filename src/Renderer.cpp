#include "Renderer.hpp"

Renderer::Renderer(arln::Context& t_c) noexcept : m_ctx{ t_c }{
    m_cmd = m_ctx.allocateCommandBuffer();
}

auto Renderer::drF() noexcept -> v0 {
    m_ctx.beginFrame();
    m_cmd.begin();
    m_cmd.transitionImages(arln::ImageTransitionInfo{
        .image = m_ctx.getPresentImage(), .oldLayout = arln::ImageLayout::eUndefined,
        .newLayout = arln::ImageLayout::eColorAttachment, .srcStageMask = arln::PipelineStageBits::eTopOfPipe,
        .dstStageMask = arln::PipelineStageBits::eColorAttachmentOutput, .srcAccessMask = 0,
        .dstAccessMask = arln::AccessBits::eColorAttachmentWrite
    });
    arln::ColorAttachmentInfo ca{ .clearColor = { 1, 0.25, 0.75, 1}, .image = m_ctx.getPresentImage()};
    m_cmd.beginRendering(arln::RenderingInfo{.pColorAttachment = &ca});
    m_cmd.endRendering();
    m_cmd.transitionImages(arln::ImageTransitionInfo{
        .image = m_ctx.getPresentImage(), .oldLayout = arln::ImageLayout::eColorAttachment,
        .newLayout = arln::ImageLayout::ePresentSrc, .srcStageMask = arln::PipelineStageBits::eColorAttachmentOutput,
        .dstStageMask = arln::PipelineStageBits::eBottomOfPipe,
        .srcAccessMask = arln::AccessBits::eColorAttachmentWrite, .dstAccessMask = 0
    });
    m_cmd.end();
    m_ctx.endFrame({ m_cmd });
}

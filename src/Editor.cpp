#include "Editor.hpp"

Editor::Editor(arln::Window& t_w, arln::Context& t_c) noexcept : m_wnd{ t_w }, m_ctx{ t_c } {
    arln::ImguiContext::Init(t_w);
    m_cmd = m_ctx.allocateCommandBuffer();
}

Editor::~Editor() noexcept {
    arln::ImguiContext::Terminate();
}

auto Editor::e() noexcept -> v0 {
    ImGui::ShowDemoWindow();
}

auto Editor::r() noexcept -> v0 {
    e();
    auto colorAttachment = arln::ColorAttachmentInfo{
        .image = m_ctx.getPresentImage(),
    }; m_cmd.begin();
    m_cmd.transitionImages(arln::ImageTransitionInfo{
        .image = m_ctx.getPresentImage(),
        .oldLayout = arln::ImageLayout::eUndefined, .newLayout = arln::ImageLayout::eColorAttachment,
        .srcStageMask = arln::PipelineStageBits::eTopOfPipe, .dstStageMask = arln::PipelineStageBits::eColorAttachmentOutput,
        .srcAccessMask = arln::AccessBits::eNone, .dstAccessMask = arln::AccessBits::eColorAttachmentWrite
    }); m_cmd.beginRendering(arln::RenderingInfo{
        .pColorAttachment = &colorAttachment
    });
    arln::ImguiContext::Render(m_cmd);
    m_cmd.endRendering();
    m_cmd.transitionImages(arln::ImageTransitionInfo{
        .image = m_ctx.getPresentImage(),
        .oldLayout = arln::ImageLayout::eColorAttachment, .newLayout = arln::ImageLayout::ePresentSrc,
        .srcStageMask = arln::PipelineStageBits::eColorAttachmentOutput, .dstStageMask = arln::PipelineStageBits::eBottomOfPipe,
        .srcAccessMask = arln::AccessBits::eColorAttachmentWrite, .dstAccessMask = arln::AccessBits::eNone
    });
    m_cmd.end();
}
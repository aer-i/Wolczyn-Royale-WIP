#include "Editor.hpp"
#include "Time.hpp"

Editor::Editor(arln::Window& t_w, arln::Context& t_c) noexcept : m_wnd{ t_w }, m_ctx{ t_c } {
    arln::ImguiContext::Init(t_w);
}

Editor::~Editor() noexcept {
    arln::ImguiContext::Terminate();
}

auto Editor::e() noexcept -> v0 {

}

auto Editor::r(arln::CommandBuffer& t_cmd) noexcept -> v0 {
    e(); auto colorAttachment = arln::ColorAttachmentInfo{
        .image = m_ctx.getPresentImage(),
        .late = true
    }; t_cmd.begin(); t_cmd.transitionImages({arln::ImageTransitionInfo{
        .image = m_ctx.getPresentImage(),
        .oldLayout = arln::ImageLayout::eUndefined, .newLayout = arln::ImageLayout::eColorAttachment,
        .srcStageMask = arln::PipelineStageBits::eColorAttachmentOutput, .dstStageMask = arln::PipelineStageBits::eColorAttachmentOutput,
        .srcAccessMask = arln::AccessBits::eNone, .dstAccessMask = arln::AccessBits::eColorAttachmentWrite
    }}); t_cmd.beginRendering(arln::RenderingInfo{
        .pColorAttachment = &colorAttachment
    }); arln::ImguiContext::Render(t_cmd);
    t_cmd.endRendering(); t_cmd.transitionImages(arln::ImageTransitionInfo{
        .image = m_ctx.getPresentImage(),
        .oldLayout = arln::ImageLayout::eColorAttachment, .newLayout = arln::ImageLayout::ePresentSrc,
        .srcStageMask = arln::PipelineStageBits::eColorAttachmentOutput, .dstStageMask = arln::PipelineStageBits::eColorAttachmentOutput,
        .srcAccessMask = arln::AccessBits::eColorAttachmentWrite, .dstAccessMask = arln::AccessBits::eNone
    }); t_cmd.end();
}
#include "Editor.hpp"

Editor::Editor(arln::Window& t_w, arln::Context& t_c) noexcept : m_wnd{ t_w }, m_ctx{ t_c } {
    arln::ImguiContext::Init(t_w);
    m_cmd = m_ctx.allocateCommandBuffer();
}

Editor::~Editor() noexcept {
    arln::ImguiContext::Terminate();
}

auto Editor::e() noexcept -> v0 {
    auto const* v = ImGui::GetMainViewport();
    ImVec2 wp, wpp;
    wp.x = v->WorkPos.x + 10; wp.y = v->WorkPos.y + 10;
    wpp.x = {0}; wpp.y = {0};
    ImGui::SetNextWindowPos(wp, ImGuiCond_Always, wpp);
    ImGui::SetNextWindowViewport(v->ID);
    ImGui::SetNextWindowBgAlpha(0.35f);
    if (ImGui::Begin("f", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoMove
    ))
    {
        ImGui::Text("Fps overlay\nfps: %u", m_wnd.getFps());
    }
    ImGui::End();
}

auto Editor::r() noexcept -> v0 {
    e();
    auto colorAttachment = arln::ColorAttachmentInfo{
        .image = m_ctx.getPresentImage(),
        .late = true
    }; m_cmd.begin(); m_cmd.beginRendering(arln::RenderingInfo{
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
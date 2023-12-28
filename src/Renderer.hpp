#pragma once
#include <Arln.hpp>

class Renderer {
    using v0 = void;
public:
    Renderer() = delete;
    explicit Renderer(arln::Context& t_c) noexcept;
    ~Renderer() = default;
    auto drF() noexcept -> v0;
private:
    arln::Context& m_ctx;
    arln::CommandBuffer m_cmd{};
};
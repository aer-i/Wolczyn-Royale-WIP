#pragma once
#include <Arln.hpp>

class Scene
{
public:
    Scene(arln::Context& t_c) noexcept;
    ~Scene() noexcept;
    inline auto& gMt() noexcept {
        return m_mats;
    }
private:
    arln::Context& m_ctx;
    std::vector<arln::Pipeline> m_mats;
};
#pragma once
#include "Camera.hpp"
#include <Arln.hpp>

class Sky
{   using v0 = void; struct PC{
        glm::vec2 mps;
        glm::vec2 res;
    };
public:
    Sky() noexcept;
    ~Sky() noexcept;
    auto r(arln::CommandBuffer& t_cmd, Camera& t_cm) noexcept -> v0;
private:
    arln::Pipeline m_p{};
};
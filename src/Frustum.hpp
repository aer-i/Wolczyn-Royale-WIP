#pragma once
#include <Arln.hpp>

class Frustum {
    enum side { L = 0, R = 1, T = 2, B = 3, BA = 4, FR = 5 };
    using v0 = void;
public:
    auto u(glm::mat4 const& t_mtx) -> v0 {
        pln[L].x = t_mtx[0].w + t_mtx[0].x;
        pln[L].y = t_mtx[1].w + t_mtx[1].x;
        pln[L].z = t_mtx[2].w + t_mtx[2].x;
        pln[L].w = t_mtx[3].w + t_mtx[3].x;

        pln[R].x = t_mtx[0].w - t_mtx[0].x;
        pln[R].y = t_mtx[1].w - t_mtx[1].x;
        pln[R].z = t_mtx[2].w - t_mtx[2].x;
        pln[R].w = t_mtx[3].w - t_mtx[3].x;

        pln[T].x = t_mtx[0].w - t_mtx[0].y;
        pln[T].y = t_mtx[1].w - t_mtx[1].y;
        pln[T].z = t_mtx[2].w - t_mtx[2].y;
        pln[T].w = t_mtx[3].w - t_mtx[3].y;

        pln[B].x = t_mtx[0].w + t_mtx[0].y;
        pln[B].y = t_mtx[1].w + t_mtx[1].y;
        pln[B].z = t_mtx[2].w + t_mtx[2].y;
        pln[B].w = t_mtx[3].w + t_mtx[3].y;

        pln[BA].x = t_mtx[0].w + t_mtx[0].z;
        pln[BA].y = t_mtx[1].w + t_mtx[1].z;
        pln[BA].z = t_mtx[2].w + t_mtx[2].z;
        pln[BA].w = t_mtx[3].w + t_mtx[3].z;

        pln[FR].x = t_mtx[0].w - t_mtx[0].z;
        pln[FR].y = t_mtx[1].w - t_mtx[1].z;
        pln[FR].z = t_mtx[2].w - t_mtx[2].z;
        pln[FR].w = t_mtx[3].w - t_mtx[3].z;

        for (auto& p : pln) {
            arln::f32 lnh = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
            p /= lnh;
        }
    }

    [[nodiscard]] auto cS(glm::vec3 const& t_p, arln::f32 t_r) const noexcept -> bool {
        for (auto& p : pln) {
            if ((p.x * t_p.x) + (p.y * t_p.y) + (p.z * t_p.z) + p.w <= -t_r) {
                return false;
        }} return true;
    }
private:
    std::array<arln::vec4, 6> pln{};
};
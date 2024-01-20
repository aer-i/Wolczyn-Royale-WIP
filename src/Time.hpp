#pragma once
#include <chrono>
#include <Arln.hpp>

class Time {
    using v0 = void;
    static inline arln::f32 s_dt{};
    static inline arln::u32 s_fr{};
public:
    static auto u() noexcept -> v0 {
        static auto pt = gT();
        auto ct = gT();
        s_dt = static_cast<arln::f32>(ct) - static_cast<arln::f32>(pt);
        pt = ct;
        static auto fpt = gT();
        static unsigned ffc = 0;
        ffc++; if (ct - fpt >= 1.0) {
            s_fr = ffc;
            ffc = 0;
            fpt = ct;
        }
    } static inline auto gDt() noexcept -> arln::f32 {
        return s_dt;
    } static inline auto gFr() noexcept -> arln::u32 {
        return s_fr;
    } static auto gT() noexcept -> arln::f64 {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = now - startTime;
        return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    }
};
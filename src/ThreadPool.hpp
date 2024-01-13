#pragma once
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>

class Thread {
    using v0 = void;
public:
    Thread() noexcept;
    ~Thread() noexcept;
    auto enq(std::function<void()> const& t_t) noexcept -> v0;
    auto w() noexcept -> v0;
private:
    auto tl() noexcept -> v0;
private:
    std::thread m_t;
    std::queue<std::function<void()>> m_jq;
    std::mutex m_qm;
    std::condition_variable m_cv;
    bool m_e = false;
};

class ThreadPool {
    using u32 = unsigned;
    using v0 = void;
public:
    explicit ThreadPool(u32 t_tc = std::thread::hardware_concurrency()) {
        m_ths = std::vector<Thread>(t_tc);
    }
    inline auto enq(std::function<void()> const& t_t) noexcept -> v0 {
        m_ths[m_ct].enq(t_t);
        m_ct = (m_ct + 1) % m_ths.size();
    } inline auto w() noexcept -> v0 {
        for (auto& t : m_ths) {
            t.w();
        }
    }
private:
    std::vector<Thread> m_ths;
    u32 m_ct{};
};
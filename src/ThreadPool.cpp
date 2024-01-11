#include "ThreadPool.hpp"

Thread::Thread() noexcept {
    m_t = std::thread(&Thread::tl, this);
}

Thread::~Thread() noexcept {
    if (m_t.joinable()) {
        this->w();
        m_qm.lock();
        m_e = true;
        m_cv.notify_one();
        m_qm.unlock();
        m_t.join();
    }
}

auto Thread::enq(std::function<void()> const& t_t) noexcept -> v0 {
    std::lock_guard<std::mutex> l(m_qm);
    m_jq.push(t_t);
    m_cv.notify_one();
}

auto Thread::w() noexcept -> v0 {
    std::unique_lock<std::mutex> l(m_qm);
    m_cv.wait(l, [this]{ return m_jq.empty(); });
}

auto Thread::tl() noexcept -> v0 {
    while (true) {
        std::function<void()> t;
        {
            std::unique_lock<std::mutex> l(m_qm);
            m_cv.wait(l, [this] { return !m_jq.empty() || m_e; });
            if (m_e) {
                break;
            }
            t = m_jq.front();
        }
        t();
        {
            std::lock_guard<std::mutex> l(m_qm);
            m_jq.pop();
            m_cv.notify_one();
        }
    }
}

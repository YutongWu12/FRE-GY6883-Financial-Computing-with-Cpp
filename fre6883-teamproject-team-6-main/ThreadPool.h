// by Wenbo Gao
#pragma once
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    explicit ThreadPool(std::size_t n) {
        for (std::size_t i = 0; i < n; ++i)
            workers.emplace_back([this] {
                for (;;) {
                    std::function<void()> job;
                    {
                        std::unique_lock<std::mutex> lk(mu);
                        cv.wait(lk, [&]{ return stop || !q.empty(); });
                        if (stop && q.empty()) return;
                        job = std::move(q.front());
                        q.pop();
                    }
                    job();
                }
            });
    }

    template<class F>
    void enqueue(F&& f) {
        {
            std::lock_guard<std::mutex> lk(mu);
            q.emplace(std::forward<F>(f));
        }
        cv.notify_one();
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lk(mu);
            stop = true;
        }
        cv.notify_all();
        for (auto &w : workers) w.join();
    }

private:
    std::vector<std::thread>          workers;
    std::queue<std::function<void()>> q;
    std::mutex                        mu;
    std::condition_variable           cv;
    bool                              stop = false;
};

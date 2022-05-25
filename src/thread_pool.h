// Copyright 2022. All Rights Reserved.
// Author: Bruce-Lee-LY
// Date: 22:25:00 on Tue, May 17, 2022
//
// Description: thread pool

#ifndef __THREAD_POOL_THREAD_POOL_H__
#define __THREAD_POOL_THREAD_POOL_H__

#include <atomic>
#include <functional>
#include <future>
#include <thread>
#include <vector>

#include "task_queue.h"

template <size_t task_queue_size>
class ThreadPool {
public:
    using thread_task = std::function<void()>;

    ThreadPool(size_t thread_size) : m_running(true) {
        for (size_t i = 0; i < thread_size; ++i) {
            m_threads.emplace_back([this] {
                while (m_running) {
                    thread_task task = m_tasks.Consume();
                    if (task && m_running) {
                        task();
                    }
                }
            });
        }
    }

    ~ThreadPool() {
        m_running = false;
        m_tasks.Destroy();

        for (auto iter = m_threads.begin(); iter != m_threads.end();) {
            if (iter->joinable()) {
                iter->join();
            }

            iter = m_threads.erase(iter);
        }
        std::vector<std::thread>().swap(m_threads);
    }

    template <class F, class... Args>
    auto AddTask(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using ret_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<ret_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<ret_type> res = task->get_future();
        if (m_running) {
            m_tasks.Produce([task]() { (*task)(); });
        }

        return res;
    }

    template <class F, class... Args>
    auto AddPriorTask(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using ret_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<ret_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<ret_type> res = task->get_future();
        if (m_running) {
            m_tasks.ProducePrior([task]() { (*task)(); });
        }

        return res;
    }

private:
    ThreadPool(const ThreadPool &) = delete;
    const ThreadPool &operator=(const ThreadPool &) = delete;

    std::atomic<bool> m_running{false};

    std::vector<std::thread> m_threads;
    TaskQueue<thread_task, task_queue_size> m_tasks;
};

#endif  // __THREAD_POOL_THREAD_POOL_H__

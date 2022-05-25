// Copyright 2022. All Rights Reserved.
// Author: Bruce-Lee-LY
// Date: 22:28:35 on Wed, May 18, 2022
//
// Description: task queue

#ifndef __THREAD_POOL_TASK_QUEUE_H__
#define __THREAD_POOL_TASK_QUEUE_H__

#include <condition_variable>
#include <deque>
#include <mutex>

template <typename T, size_t max_size>
class TaskQueue {
public:
    TaskQueue() = default;
    ~TaskQueue() = default;

    void Produce(T task) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this] { return !full() || !m_running; });

            if (!full() && m_running) {
                m_deque.push_back(std::move(task));
            }
        }
        m_cv.notify_one();
    }

    void ProducePrior(T task) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this] { return !full() || !m_running; });

            if (!full() && m_running) {
                m_deque.push_front(std::move(task));
            }
        }
        m_cv.notify_one();
    }

    T Consume() {
        T task = T();
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this] { return !m_deque.empty() || !m_running; });

            if (!m_deque.empty() && m_running) {
                task = std::move(m_deque.front());
                m_deque.pop_front();
            }
        }
        m_cv.notify_one();

        return std::move(task);
    }

    void Destroy() {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_running = false;
        }
        m_cv.notify_all();

        std::unique_lock<std::mutex> lock(m_mutex);
        while (!m_deque.empty()) {
            m_deque.pop_front();
        }
        std::deque<T>().swap(m_deque);
    }

private:
    TaskQueue(const TaskQueue &) = delete;
    TaskQueue &operator=(const TaskQueue &) = delete;

    bool full() const {
        return m_deque.size() >= m_max_size;
    }

    std::deque<T> m_deque;
    size_t m_max_size = max_size;

    bool m_running = true;
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
};

#endif  // __THREAD_POOL_TASK_QUEUE_H__

// Copyright 2022. All Rights Reserved.
// Author: Bruce-Lee-LY
// Date: 22:27:17 on Tue, May 17, 2022
//
// Description: test thread pool

#include <chrono>
#include <iostream>
#include <vector>

#include "thread_pool.h"

#define TP_THREAD_SIZE 5
#define TP_TASK_QUEUE_SIZE 10
#define TP_TEST_CYCLE_SIZE 20

int main() {
    ThreadPool<TP_TASK_QUEUE_SIZE> tp(TP_THREAD_SIZE);
    std::vector<std::future<int>> results;

    for (int i = 0; i < TP_TEST_CYCLE_SIZE; ++i) {
        results.emplace_back(tp.AddTask([i] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "thread[" << std::this_thread::get_id() << "] " << i << std::endl;
            return i * i;
        }));

        results.emplace_back(tp.AddPriorTask([i] {
            std::cout << "thread[" << std::this_thread::get_id() << "] " << -i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return -i * i;
        }));
    }

    for (auto &&result : results) {
        std::cout << result.get() << std::endl;
    }

    return 0;
}

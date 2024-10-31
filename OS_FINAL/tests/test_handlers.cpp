#include "doctest.h"
#include "../servers/leader_follower/lf_handler.hpp"
#include "../servers/pipeline/pipeline_handler.hpp"

TEST_CASE("Leader follower") {
    LFHandler lf(10);
    std::atomic<int> tasks_done = 0;
    for (int i = 0; i < 20; i++) {
        lf.add_task([i, &tasks_done]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            tasks_done.fetch_add(1, std::memory_order_relaxed);
        });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    CHECK(tasks_done == 20);
}
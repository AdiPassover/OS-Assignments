#include <iostream>
#include <thread>
#include <mutex>
#include "guard.hpp"

std::mutex counter_mutex;
int global_counter = 0;

void global_increment() {
    Guard guard(counter_mutex);
    global_counter++;
}

int main() {
    std::cout << "Incrementing shared data 100000 times: " << std::endl;
    for (int i = 0; i < 100000; i++) {
        std::thread t(global_increment);
        t.detach();
    }
    std::cout << "Waiting for threads to finish..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "Final shared data: " << global_counter << std::endl;

    return 0;
}

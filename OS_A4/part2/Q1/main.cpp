#include "singleton.hpp"
#include <iostream>
#include <thread>

void thread1_func(int num) {
    Singleton<int>& s = Singleton<int>::get_instance();

    std::string msg = "thread1 data: " + std::to_string(s.get_data());
    std::cout << msg << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    msg = "thread1 setting data to: " + std::to_string(num);
    std::cout << msg << std::endl;
    s.set_data(num);

    msg = "thread1 data: " + std::to_string(s.get_data());
    std::cout << msg << std::endl;
}

void thread2_func() {
    Singleton<int>& s = Singleton<int>::get_instance();

    std::string msg = "thread2 data: " + std::to_string(s.get_data());
    std::cout << msg << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    msg = "thread2 data: " + std::to_string(s.get_data());
    std::cout << msg << std::endl;
}

int main() {
    Singleton<int>& s1 = Singleton<int>::get_instance();
    Singleton<int>& s2 = Singleton<int>::get_instance();

    std::cout << "setting s1 to 5" << std::endl;
    s1.set_data(5);
    std::cout << "s1: " << s1.get_data() << " s2: " << s2.get_data() << std::endl;
    std::cout << "setting s2 to 10" << std::endl;
    s2.set_data(10);
    std::cout << "s1: " << s1.get_data() << " s2: " << s2.get_data() << std::endl;

    std::thread t1(thread1_func, 15);
    std::thread t2(thread2_func);

    t1.join();
    t2.join();

    return 0;
}
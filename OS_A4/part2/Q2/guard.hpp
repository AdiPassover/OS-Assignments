#ifndef OS_A4_GUARD_HPP
#define OS_A4_GUARD_HPP

#include <mutex>

class Guard {
public:
    inline Guard(std::mutex& mtx) : _mutex(mtx) { _mutex.lock(); }
    inline ~Guard() { _mutex.unlock(); }

    Guard(const Guard&) = delete;
    Guard& operator=(const Guard&) = delete;

private:
    std::mutex& _mutex;

};


#endif //OS_A4_GUARD_HPP

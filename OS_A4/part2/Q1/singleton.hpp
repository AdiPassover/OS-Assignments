#ifndef OS_A4_SINGLETON_HPP
#define OS_A4_SINGLETON_HPP

#include <mutex>

template <typename T>
class Singleton {
public:
    inline static Singleton<T>& get_instance() {
        if (_instance == nullptr) {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_instance == nullptr) _instance = new Singleton<T>();
        }
        return *_instance;
    }

    inline static void destroy_instance() {
        std::lock_guard<std::mutex> lock(_mutex);
        delete _instance;
        _instance = nullptr;
    }

    inline const T& get_data() { return _data; }
    inline void set_data(const T& data) { _data = data; }

private:
    static Singleton<T>* _instance;
    static std::mutex _mutex;

    T _data;

    inline Singleton() = default;

};

template <typename T>
Singleton<T>* Singleton<T>::_instance = nullptr;

template <typename T>
std::mutex Singleton<T>::_mutex;


#endif //OS_A4_SINGLETON_HPP

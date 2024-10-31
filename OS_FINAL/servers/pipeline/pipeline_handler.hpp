#ifndef OS_FINAL_PIPELINE_HANDLER_HPP
#define OS_FINAL_PIPELINE_HANDLER_HPP

#include <functional>
#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

template<typename T>
class PipelineHandler {
public:
    using ProcessFunction = std::function<T(const T&)>;

    PipelineHandler();
    ~PipelineHandler();

    PipelineHandler& add_stage(const std::string& key, const ProcessFunction& processFunc);
    PipelineHandler& add_consecutive_stages(const std::string& key, const std::initializer_list<ProcessFunction> &processFuncs);

    void run_pipeline(const std::string &key, const T &data);
    void stop();

private:
    class PipelineStage { // Active Object pattern
    public:
        using TaskFunction = std::function<void()>;

        explicit PipelineStage(const ProcessFunction& processFunction, const std::shared_ptr<PipelineStage>& next_stage = nullptr);
        ~PipelineStage();

        void run_stage(const T& data);
        void run();
        void stop();

    private:
        PipelineStage& add_task(TaskFunction task); // returns reference to itself for chaining

    private:
        std::thread _thread;
        std::atomic<bool> _running;
        std::queue<TaskFunction> _tasks;
        std::mutex _tasks_mutex;
        std::condition_variable _new_task_cv;

        std::shared_ptr<PipelineStage> _next_stage;
        ProcessFunction _process;
    };

private:
    std::unordered_map<std::string, std::shared_ptr<PipelineStage>> _pipelines;


};

template<typename T>
PipelineHandler<T>::PipelineStage::PipelineStage(const ProcessFunction& processFunction, const std::shared_ptr<PipelineStage>& next_stage)
        : _running(true), _next_stage(next_stage), _process(processFunction) {
    _thread = std::thread(&PipelineHandler::PipelineStage::run, this);
}

template<typename T>
PipelineHandler<T>::PipelineStage::~PipelineStage() { stop(); }

template<typename T>
void PipelineHandler<T>::PipelineStage::run() {
    while (true) {
        TaskFunction task;
        {
            std::unique_lock<std::mutex> lock(_tasks_mutex);
            _new_task_cv.wait(lock, [this] { return !_tasks.empty() || !_running; });

            if (!_running && _tasks.empty()) return;
            if (!_tasks.empty()) {
                task = std::move(_tasks.front());
                _tasks.pop();
            }
        }
        if (task) task();
    }
}

template<typename T>
void PipelineHandler<T>::PipelineStage::stop() {
    {
        std::lock_guard<std::mutex> lock(_tasks_mutex);
        _running = false;
        _new_task_cv.notify_all();
    }
    if (_thread.joinable()) _thread.join();
}

template<typename T>
typename PipelineHandler<T>::PipelineStage& PipelineHandler<T>::PipelineStage::add_task(TaskFunction task) {
    {
        std::lock_guard<std::mutex> lock(_tasks_mutex);
        _tasks.push(std::move(task));
        _new_task_cv.notify_one();
    }
    return *this;
}

template<typename T>
void PipelineHandler<T>::PipelineStage::run_stage(const T& data) {
    add_task([this, data] {
        try {
            T response = _process(data);
            if (_next_stage != nullptr) _next_stage->run_stage(response);
        } catch (const std::exception& ex) {
            T response = {ex.what(), data};
            if (_next_stage != nullptr) _next_stage->run_stage(response);
        }
    });
}

// ===================

template<typename T>
PipelineHandler<T>::PipelineHandler() : _pipelines() {}

template<typename T>
PipelineHandler<T>::~PipelineHandler() { stop(); }

template<typename T>
PipelineHandler<T>& PipelineHandler<T>::add_stage(const std::string& key, const ProcessFunction& processFunc) {
    _pipelines[key] = std::make_shared<PipelineStage>(processFunc);
    return *this;
}

template<typename T>
PipelineHandler<T>& PipelineHandler<T>::add_consecutive_stages(const std::string& key, const std::initializer_list<ProcessFunction> &processFuncs) {
    std::shared_ptr<PipelineStage> curr = nullptr;
    for (auto it = rbegin(processFuncs); it != rend(processFuncs); ++it) {
        curr = std::make_shared<PipelineStage>(*it, curr);
    }
    _pipelines[key] = curr;
    return *this;
}

template<typename T>
void PipelineHandler<T>::run_pipeline(const std::string &key, const T &data) {
    _pipelines[key]->run_stage(data);
}

template<typename T>
void PipelineHandler<T>::stop() {
    for (auto& pair : _pipelines) {
        pair.second->stop();
    }
}


#endif //OS_FINAL_PIPELINE_HANDLER_HPP
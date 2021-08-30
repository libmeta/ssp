//
// Created by TBD on 2020/8/28.
//

#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>

namespace ssp::base {

class Semaphore {
private:
    Semaphore(const Semaphore &) = delete;

    Semaphore(Semaphore &&) = delete;

    Semaphore &operator=(const Semaphore &) = delete;

    Semaphore &operator=(Semaphore &&) = delete;

public:
    Semaphore(int init_val = 0);

    ~Semaphore();

    void Wait();

    bool TryWait();

    template<typename D>
    bool TimedWait(const D &duration);

    bool TimedWait(const std::chrono::time_point <std::chrono::steady_clock> &abs_time);

    void Post();

private:
    int64_t mCurCount;
    std::mutex mMutex;
    std::condition_variable mCondVar;
};

}

#include "semaphore.inl"




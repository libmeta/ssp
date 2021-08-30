
#include "semaphore.hpp"
#include "core/common.hpp"

namespace xlab::base {

Semaphore::Semaphore(int init_val) : mCurCount(init_val) {

}

Semaphore::~Semaphore() {

}

void Semaphore::Wait() {
    while (true) {
        std::unique_lock <std::mutex> lock{mMutex};
        if (mCurCount > 0) {
            --mCurCount;
            break;
        }
        mCondVar.wait(lock);
        if (mCurCount > 0) {
            --mCurCount;
            break;
        }
    }
}

bool Semaphore::TryWait() {
    std::unique_lock <std::mutex> lock{mMutex};
    if (mCurCount > 0) {
        --mCurCount;
        return true;
    }

    mCondVar.wait_for(lock, std::chrono::nanoseconds(0));
    if (mCurCount > 0) {
        --mCurCount;
        return true;
    }

    return false;
}

bool Semaphore::TimedWait(const std::chrono::time_point <std::chrono::steady_clock> &abs_time) {
    std::unique_lock <std::mutex> lock{mMutex};
    if (mCurCount > 0) {
        --mCurCount;
        return true;
    }

    mCondVar.wait_until(lock, abs_time);
    if (mCurCount > 0) {
        --mCurCount;
        return true;
    }

    return false;
}

void Semaphore::Post() {
    std::lock_guard <std::mutex> lock{mMutex};
    ++mCurCount;
    mCondVar.notify_one();
}

}


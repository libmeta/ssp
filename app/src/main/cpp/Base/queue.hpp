#pragma once

#include <mutex>
#include <queue>
#include <list>
#include <atomic>
#include <shared_mutex>
#include <optional>
#include "semaphore.hpp"

namespace xlab {
#ifdef __APPLE__
#define SEM_VALUE_MAX 2*1024
#endif
template<typename E>
class CQueue {
private:
    CQueue(const CQueue &) = delete;

    CQueue(CQueue &&) = delete;

    CQueue &operator=(const CQueue &) = delete;

    CQueue &operator=(CQueue &&) = delete;

public:
    CQueue(int max = 3) : mMaxCount(max), mSemapIn(max), mSemapOut(0) {}

    ~CQueue() = default;

    bool Push(const E &element, int milliseconds = 0) {
        if (milliseconds <= 0) {
            if (mSemapIn.TryWait()) {
                {
                    std::lock_guard<decltype(mMutex)> locker(mMutex);
                    mQueue.push(element);
                }
                mSemapOut.Post();

                return true;
            }
        } else {
            if (mSemapIn.TimedWait(std::chrono::milliseconds(milliseconds))) {
                {
                    std::lock_guard<decltype(mMutex)> locker(mMutex);
                    mQueue.push(element);
                }
                mSemapOut.Post();

                return true;
            }
        }

        return false;
    }

    std::optional<E> Front() {
        std::lock_guard<decltype(mMutex)> locker(mMutex);
        if (mQueue.empty()) {
            return std::nullopt;
        }

        return std::make_optional(mQueue.front());
    }

    std::optional<E> Pop(int milliseconds = 0) {
        if (milliseconds <= 0) {
            if (mSemapOut.TryWait()) {
                std::optional<E> front = std::nullopt;
                {
                    std::lock_guard<decltype(mMutex)> lock(mMutex);
                    front = std::make_optional(mQueue.front());
                    mQueue.pop();
                }
                mSemapIn.Post();

                return front;
            }
        } else {
            if (mSemapOut.TimedWait(std::chrono::milliseconds(milliseconds))) {
                std::optional<E> front = std::nullopt;
                {
                    std::lock_guard<decltype(mMutex)> lock(mMutex);
                    front = std::make_optional(mQueue.front());
                    mQueue.pop();
                }
                mSemapIn.Post();

                return front;
            }
        }

        return std::nullopt;
    }

    size_t Size() {
        std::shared_lock<decltype(mMutex)> locker(mMutex);
        return mQueue.size();
    }

    bool Empty() {
        std::shared_lock<decltype(mMutex)> locker(mMutex);
        return mQueue.empty();
    }

    bool Full() {
        std::shared_lock<decltype(mMutex)> locker(mMutex);
        return mQueue.size() >= mMaxCount;
    }

private:
    int mMaxCount;
    CSemaphore mSemapIn;
    CSemaphore mSemapOut;
    std::shared_mutex mMutex;
    std::queue<E, std::list<E>> mQueue;
};

}

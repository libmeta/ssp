#pragma once

#include <list>
#include <queue>
#include <optional>
#include <shared_mutex>
#include "semaphore.hpp"

namespace ssp::base {

template<typename E>
class CBlockQueue {

public:
    using value_type = E;
    using reference = value_type &;
    using const_reference = const value_type &;

private:
    CBlockQueue(const CBlockQueue &) = delete;

    CBlockQueue(CBlockQueue &&) = delete;

    CBlockQueue &operator=(const CBlockQueue &) = delete;

    CBlockQueue &operator=(CBlockQueue &&) = delete;

public:
    CBlockQueue(int max = 3) : mMaxCount(max), mSemapIn(max), mSemapOut(0) {}

    ~CBlockQueue() = default;

    bool TryPush(const E &element) {
        if (mSemapIn.TryWait()) {
            std::lock_guard<decltype(mMutex)> locker(mMutex);
            mQueue.push(element);
            mSemapOut.Post();
            return true;
        }
        return false;
    }

    void Push(const E &element) {
        mSemapIn.Wait();
        std::lock_guard<decltype(mMutex)> locker(mMutex);
        mQueue.push(element);
        mSemapOut.Post();
    }

    void Pulse() {
        mSemapIn.Post();
        mSemapOut.Post();
    }

    std::optional<E> TryPop() {
        if (mSemapOut.TryWait()) {
            std::optional<E> front = std::nullopt;
            std::lock_guard<decltype(mMutex)> lock(mMutex);
            front = std::make_optional(mQueue.front());
            mQueue.pop();
            mSemapIn.Post();
            return front;
        }
        return std::nullopt;
    }

    void Clear() {
        std::lock_guard<decltype(mMutex)> lock(mMutex);
        while (!mQueue.empty()) {
            mQueue.pop();
        }
    }

    void ClearIf(std::function<bool(E&)> func) {
        std::lock_guard<decltype(mMutex)> lock(mMutex);
        decltype(mQueue) newQueue;
        while (!mQueue.empty()) {
            auto element = mQueue.front();
            mQueue.pop();
            if (!func(element)) {
                newQueue.push(element);
            }
        }
        mQueue = newQueue;
    }

    std::optional<E> Pop() {
        mSemapOut.Wait();
        std::lock_guard<decltype(mMutex)> lock(mMutex);
        if (mQueue.empty()) {
            mSemapIn.Post();
            return std::nullopt;
        }
        auto front = std::make_optional(mQueue.front());
        mQueue.pop();
        mSemapIn.Post();
        return front;
    }

    std::optional<E> Front() {
        std::lock_guard<decltype(mMutex)> lock(mMutex);
        if (mQueue.empty()) {
            return std::nullopt;
        }
        return std::make_optional(mQueue.front());
    }

    size_t Size() {
        std::lock_guard<decltype(mMutex)> locker(mMutex);
        return mQueue.size();
    }

    bool Empty() {
        std::lock_guard<decltype(mMutex)> locker(mMutex);
        return mQueue.empty();
    }

    bool Full() {
        std::lock_guard<decltype(mMutex)> locker(mMutex);
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

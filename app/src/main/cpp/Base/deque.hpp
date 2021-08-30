//
// Created by king on 21-1-2.
//

#ifndef XLIVE_DEQUE_HPP
#define XLIVE_DEQUE_HPP

#include <mutex>
#include <queue>
#include <list>
#include <atomic>
#include <shared_mutex>
#include <optional>
#include "semaphore.hpp"

namespace xlab {

template<typename E>
class CDeque {
private:
    CDeque(const CDeque &) = delete;

    CDeque(CDeque &&) = delete;

    CDeque &operator=(const CDeque &) = delete;

    CDeque &operator=(CDeque &&) = delete;

public:
    CDeque() {}

    ~CDeque() = default;

    bool PushFront(const E &element) {
        {
            std::lock_guard<decltype(mMutex)> locker(mMutex);
            mDeque.push_front(element);
        }
        mSemap.Post();

        return false;
    }

    bool PushBack(const E &element) {
        {
            std::lock_guard<decltype(mMutex)> locker(mMutex);
            mDeque.push_back(element);
        }
        mSemap.Post();

        return false;
    }

    std::optional <E>
    Front() {
        std::lock_guard<decltype(mMutex)> locker(mMutex);
        if (mDeque.empty()) {
            return std::nullopt;
        }

        return std::make_optional(mDeque.front());
    }

    std::optional <E>
    Back() {
        std::lock_guard<decltype(mMutex)> locker(mMutex);
        if (mDeque.empty()) {
            return std::nullopt;
        }

        return std::make_optional(mDeque.back());
    }

    std::optional <E> PopFront(int msec = 0) {
        if (msec <= 0) {
            if (mSemap.TryWait()) {
                std::optional <E> front = std::nullopt;
                {
                    std::lock_guard<decltype(mMutex)> lock(mMutex);
                    front = std::make_optional(mDeque.front());
                    mDeque.pop_front();
                }

                return front;
            }
        } else {
            if (mSemap.TimedWait(msec * 1000 * 1000)) {
                std::optional <E> front = std::nullopt;
                {
                    std::lock_guard<decltype(mMutex)> lock(mMutex);
                    front = std::make_optional(mDeque.front());
                    mDeque.pop_front();
                }

                return front;
            }
        }

        return std::nullopt;
    }

    std::optional <E> PopBack(int msec = 0) {
        if (msec <= 0) {
            if (mSemap.TryWait()) {
                std::optional <E> front = std::nullopt;
                {
                    std::lock_guard<decltype(mMutex)> lock(mMutex);
                    front = std::make_optional(mDeque.front());
                    mDeque.pop_back();
                }

                return front;
            }
        } else {
            if (mSemap.TimedWait(msec * 1000 * 1000)) {
                std::optional <E> front = std::nullopt;
                {
                    std::lock_guard<decltype(mMutex)> lock(mMutex);
                    front = std::make_optional(mDeque.front());
                    mDeque.pop_back();
                }

                return front;
            }
        }

        return std::nullopt;
    }

    size_t Size() {
        std::shared_lock<decltype(mMutex)> locker(mMutex);
        return mDeque.size();
    }

    bool Empty() {
        std::shared_lock<decltype(mMutex)> locker(mMutex);
        return mDeque.empty();
    }

private:
    CSemaphore mSemap;
    std::shared_mutex mMutex;
    std::deque <E> mDeque;
};

}

#endif //XLIVE_DEQUE_HPP

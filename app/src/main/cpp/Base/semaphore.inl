//
// Created by TBD on 1/18/21.
//

#pragma once

namespace ssp::base {

template<typename D>
bool Semaphore::TimedWait(const D &duration) {
    auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    if (nano < std::chrono::nanoseconds::zero()) {
        return false;
    }
    std::unique_lock <std::mutex> lock{mMutex};
    if (mCurCount > 0) {
        --mCurCount;
        return true;
    }

    mCondVar.wait_for(lock, nano);
    if (mCurCount > 0) {
        --mCurCount;
        return true;
    }

    return false;
}

}

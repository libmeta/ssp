//
// Created by x on 2021/8/18.
//

#pragma once

#include <chrono>

namespace ssp ::base {

using namespace std::chrono;

struct Time final : steady_clock {
    using nano = nanoseconds;
    using micro = microseconds;
    using milli = milliseconds;
    using steady_clock::now;

    template<typename T = micro>
    static inline auto now() {
        return duration_cast<T>(now().time_since_epoch());
    }

};
}

//
// Created by z on 2021/8/22.
//

#include "time.hpp"

namespace ssp::Time {

std::chrono::time_point <std::chrono::steady_clock> GetTimePoint() {
  return std::chrono::steady_clock::now();
}

/// 纳秒
std::chrono::nanoseconds GetNanoSeconds() {
  return std::chrono::duration_cast<decltype(GetNanoSeconds())>(std::chrono::steady_clock::now().time_since_epoch());
}

/// 微秒
std::chrono::microseconds GetMicroSeconds() {
  return std::chrono::duration_cast<decltype(GetMicroSeconds())>(std::chrono::steady_clock::now().time_since_epoch());
}

/// 毫秒
std::chrono::milliseconds GetMilliSeconds() {
  return std::chrono::duration_cast<decltype(GetMilliSeconds())>(std::chrono::steady_clock::now().time_since_epoch());
}

}

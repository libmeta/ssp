//
// Created by z on 2021/8/22.
//

#pragma once

#include <chrono>

namespace ssp::Time {

using namespace std::chrono;

using TP = std::chrono::time_point<std::chrono::steady_clock>;
using NS = std::chrono::nanoseconds;
using US = std::chrono::microseconds;
using MS = std::chrono::milliseconds;
using S = std::chrono::seconds;

enum : uint64_t {
  NS_TIME_BASE = 1000'000'000ULL,
  US_TIME_BASE = 1000'000ULL,
  MS_TIME_BASE = 1000ULL,
  S_TIME_BASE = 1ULL,
};

extern TP GetTimePoint();

extern NS GetNanoSeconds();

extern US GetMicroSeconds();

extern MS GetMilliSeconds();
}



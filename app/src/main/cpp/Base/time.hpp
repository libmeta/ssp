//
// Created by z on 2021/8/22.
//

#pragma once

#include <chrono>

namespace ssp {
template<typename CLOCK = std::chrono::steady_clock>
struct Time final : CLOCK {
  using ns = std::chrono::nanoseconds;
  using us = std::chrono::microseconds;
  using ms = std::chrono::milliseconds;
  using s = std::chrono::seconds;

  template<typename T = us>
  static inline T now() {
	return std::chrono::duration_cast<T>(CLOCK::now().time_since_epoch());
  }

};

}


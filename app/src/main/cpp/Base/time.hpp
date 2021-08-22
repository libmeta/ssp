//
// Created by z on 2021/8/22.
//

#pragma once

#include <chrono>

namespace ssp {

struct Time final : std::chrono::time_point<std::chrono::steady_clock> {
  using base = std::chrono::time_point<std::chrono::steady_clock>;

  using NS = std::chrono::nanoseconds;
  using US = std::chrono::microseconds;
  using MS = std::chrono::milliseconds;

  static inline base tp() {
	return clock::now();
  }

  template<typename T>
  static inline T now() {
	return std::chrono::duration_cast<T>(tp().time_since_epoch());
  }

};
}


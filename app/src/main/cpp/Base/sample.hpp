//
// Created by z on 2021/8/23.
//

#pragma once

#include "time.hpp"

namespace ssp {

struct Sample {
  using TickType = Time::US;
  static constexpr uint32_t NUM = 100;
  static constexpr auto TIME = TickType(TickType::period::den);

  static inline Sample zero() { return {}; }
  static inline Sample now() { return {Time::now<TickType>()}; };

  TickType tick = TickType::zero();
  uint64_t size = 0;
  uint8_t *data = nullptr;
};

}
//
// Created by z on 2021/8/22.
//

#pragma once

#include <vector>

#include "calc_sample.hpp"

namespace ssp {

class Bitrate final : public CalcSample {
 public:
  bool update(uint64_t &bitrate, uint64_t size = 0, TickType tick = Sample::now().tick);

 private:
  uint64_t currentBitrate = 0;
};

}

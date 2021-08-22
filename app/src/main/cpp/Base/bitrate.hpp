//
// Created by z on 2021/8/22.
//

#pragma once

#include <vector>

#include "sample.hpp"

namespace ssp {

class Bitrate final {
 public:
  bool update(uint64_t &bitrate, uint64_t size = 0, Sample::TickType tick = Sample::now().tick);

 private:
  Sample updateSample(Sample sample);
 private:
  uint64_t currentBitrate = 0;
  uint32_t index = 0;
  Sample::TickType lastTick = Sample::TickType::zero();
  Sample::TickType hasUpdateTick = Sample::TickType::zero();
  Sample sampleSum = Sample::zero();
  std::vector<Sample> samples;
};

}

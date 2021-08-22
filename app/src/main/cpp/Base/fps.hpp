//
// Created by z on 2021/8/22.
//

#pragma once

#include <vector>

#include "sample.hpp"

namespace ssp {

class FPS final {
 public:
  bool update(float &fps, Sample::TickType tick = Sample::now().tick);
 private:
  Sample updateSample(Sample sample);
 private:
  float currentFPS = 0;
  uint32_t index = 0;
  Sample::TickType lastTick = Sample::TickType::zero();
  Sample::TickType hasUpdateTick = Sample::TickType::zero();
  Sample sampleSum = Sample::zero();
  std::vector<Sample> samples;

};

}



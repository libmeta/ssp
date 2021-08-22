//
// Created by z on 2021/8/22.
//

#pragma once

#include <vector>

#include "time.hpp"

namespace ssp {

class Bitrate {
  static constexpr uint32_t NUM_SAMPLE = 100;
  static constexpr auto TIME_SAMPLE = Time::MS(1000);

  struct Sample {
	static inline Sample zero() { return Sample(); };
	Time::MS tick = Time::MS::zero();
	uint64_t size = 0;
  };

 public:
  bool update(uint64_t &bitrate, uint64_t size);

 private:
  Sample updateSample(Sample sample);
 private:
  uint64_t currentBitrate = 0;
  uint32_t index = 0;
  Time::MS lastMS = Time::MS::zero();
  Time::MS hasUpdateMS = Time::MS::zero();
  Sample sampleSum = Sample::zero();
  std::vector<Sample> samples;
};

}

//
// Created by z on 2021/8/23.
//

#pragma once

#include "time.hpp"

namespace ssp {

class CalcSample {
 public:
  using TickType = Time::US;
  static constexpr uint32_t NUM = 100;
  static constexpr auto TIME = TickType(TickType::period::den);

  struct Sample {
	static inline Sample zero() { return {}; }
	static inline Sample now() { return {Time::now<TickType>(), 0}; };

	TickType tick = TickType::zero();
	uint64_t size = 0;
  };

 protected:
  virtual Sample updateSample(Sample sample) {
	if (samples.size() == NUM) {
	  sampleSum.tick -= sample.tick;
	  sampleSum.size -= sample.size;
	  samples[index] = sample;
	} else {
	  samples.push_back(sample);
	}

	sampleSum.tick += sample.tick;
	sampleSum.size += sample.size;
	index = (index + 1) % NUM;

	return {sampleSum.tick / samples.size(), sampleSum.size / samples.size()};
  }

 protected:
  uint32_t index = 0;
  TickType lastTick = TickType::zero();
  TickType hasUpdateTick = TickType::zero();
  Sample sampleSum = Sample::zero();
  std::vector<Sample> samples;

};

}
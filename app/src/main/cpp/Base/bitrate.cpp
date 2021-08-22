//
// Created by z on 2021/8/22.
//

#include "bitrate.hpp"

namespace ssp {

bool Bitrate::update(uint64_t &bitrate, uint64_t size, Sample::TickType tick) {
  if (lastTick == Sample::TickType::zero()) {
	hasUpdateTick = lastTick = tick;
	bitrate = currentBitrate;
	return false;
  }

  auto dSample = updateSample({tick - lastTick, size});
  lastTick = tick;

  if (tick - hasUpdateTick >= Sample::TIME) {
	hasUpdateTick = tick;
	currentBitrate = dSample.size * 8 * Sample::TickType::period::den / dSample.tick.count();
	bitrate = currentBitrate;
	return true;
  }

  bitrate = currentBitrate;
  return false;
}

Sample Bitrate::updateSample(Sample sample) {
  if (samples.size() == Sample::NUM) {
	sampleSum.tick -= sample.tick;
	sampleSum.size -= sample.size;
	samples[index] = sample;
  } else {
	samples.push_back(sample);
  }

  sampleSum.tick += sample.tick;
  sampleSum.size += sample.size;
  index = (index + 1) % Sample::NUM;

  return {sampleSum.tick / samples.size(), sampleSum.size / samples.size()};
}

}
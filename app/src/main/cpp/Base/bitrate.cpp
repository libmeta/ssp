//
// Created by z on 2021/8/22.
//

#include "bitrate.hpp"

namespace ssp {

bool Bitrate::update(uint64_t &bitrate, uint64_t size) {
  auto nowMs = Time::GetMilliSeconds();
  if (lastMS == Time::MS::zero()) {
	hasUpdateMS = lastMS = nowMs;
	bitrate = currentBitrate;
	return false;
  }

  auto dSample = updateSample({nowMs - lastMS, size});
  lastMS = nowMs;

  if (nowMs - hasUpdateMS >= TIME_SAMPLE) {
	hasUpdateMS = nowMs;
	currentBitrate = dSample.size * 8 / dSample.tick.count();
	bitrate = currentBitrate;
	return true;
  }

  bitrate = currentBitrate;
  return false;
}

Bitrate::Sample Bitrate::updateSample(Bitrate::Sample sample) {
  if (samples.size() == NUM_SAMPLE) {
	sampleSum.tick -= sample.tick;
	sampleSum.size -= sample.size;
	samples[index] = sample;
  } else {
	samples.push_back(sample);
  }

  sampleSum.tick += sample.tick;
  sampleSum.size += sample.size;
  index = (index + 1) % NUM_SAMPLE;

  return {sampleSum.tick / samples.size(), sampleSum.size / samples.size()};
}

}
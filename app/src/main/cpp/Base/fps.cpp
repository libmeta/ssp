//
// Created by z on 2021/8/22.
//

#include "fps.hpp"

namespace ssp {

bool FPS::update(float &fps) {
  auto nowMs = Time::GetMilliSeconds();
  if (lastMS == Time::MS::zero()) {
	hasUpdateMS = lastMS = nowMs;
	fps = currentFPS;
	return false;
  }

  auto dSample = updateSample({nowMs - lastMS});
  lastMS = nowMs;

  if (nowMs - hasUpdateMS >= TIME_SAMPLE) {
	hasUpdateMS = nowMs;
	currentFPS = 1000.f / float(dSample.tick.count());
	fps = currentFPS;
	return true;
  }

  fps = currentFPS;
  return false;
}

FPS::Sample FPS::updateSample(FPS::Sample sample) {
  if (samples.size() == NUM_SAMPLE) {
	sampleSum.tick -= sample.tick;
	samples[index] = sample;
  } else {
	samples.push_back(sample);
  }

  sampleSum.tick += sample.tick;
  index = (index + 1) % NUM_SAMPLE;

  return {sampleSum.tick / samples.size()};
}

}
//
// Created by z on 2021/8/22.
//

#include "fps.hpp"

namespace ssp {

bool FPS::update(float &fps, Sample::TickType tick) {
  if (lastTick == Sample::TickType::zero()) {
	hasUpdateTick = lastTick = tick;
	fps = currentFPS;
	return false;
  }

  auto dSample = updateSample({tick - lastTick});
  lastTick = tick;

  if (tick - hasUpdateTick >= Sample::TIME) {
	hasUpdateTick = tick;
	currentFPS = float(double(Sample::TickType::period::den) / double(dSample.tick.count()));
	fps = currentFPS;
	return true;
  }

  fps = currentFPS;
  return false;
}

Sample FPS::updateSample(Sample sample) {
  if (samples.size() == Sample::NUM) {
	sampleSum.tick -= sample.tick;
	samples[index] = sample;
  } else {
	samples.push_back(sample);
  }

  sampleSum.tick += sample.tick;
  index = (index + 1) % Sample::NUM;

  return {sampleSum.tick / samples.size()};
}

}
//
// Created by z on 2021/8/22.
//

#include "fps.hpp"

namespace ssp {

bool FPS::update(float &fps, TickType tick) {
  if (lastTick == TickType::zero()) {
	hasUpdateTick = lastTick = tick;
	fps = currentFPS;
	return false;
  }

  auto dSample = updateSample({tick - lastTick});
  lastTick = tick;
  if (tick - hasUpdateTick >= TIME) {
	hasUpdateTick = tick;
	currentFPS = float(double(TickType::period::den) / double(dSample.tick.count()));
	fps = currentFPS;
	return true;
  }

  fps = currentFPS;
  return false;
}

}
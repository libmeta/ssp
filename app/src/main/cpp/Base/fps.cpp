//
// Created by z on 2021/8/22.
//

#include "fps.hpp"

namespace ssp {

bool FPS::update(float &fps, CalcSample::TICK_TYPE tick) {
  if (lastTick == CalcSample::TICK_TYPE::zero()) {
	hasUpdateTick = lastTick = tick;
	fps = currentFPS;
	return false;
  }

  auto dSample = updateSample({tick - lastTick});
  lastTick = tick;
  if (tick - hasUpdateTick >= CalcSample::TIME_INTERVAL) {
	hasUpdateTick = tick;
	currentFPS = float(double(CalcSample::TICK_TYPE::period::den) / double(dSample.tick.count()));
	fps = currentFPS;
	return true;
  }

  fps = currentFPS;
  return false;
}

}
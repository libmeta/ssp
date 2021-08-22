//
// Created by z on 2021/8/22.
//

#include "bitrate.hpp"

namespace ssp {

bool Bitrate::update(uint64_t &bitrate, uint64_t size, TickType tick) {
  if (lastTick == TickType::zero()) {
	hasUpdateTick = lastTick = tick;
	bitrate = currentBitrate;
	return false;
  }

  auto dSample = updateSample({tick - lastTick, size});
  lastTick = tick;
  if (tick - hasUpdateTick >= TIME) {
	hasUpdateTick = tick;
	currentBitrate = dSample.size * 8 * TickType::period::den / dSample.tick.count();
	bitrate = currentBitrate;
	return true;
  }

  bitrate = currentBitrate;
  return false;
}

}
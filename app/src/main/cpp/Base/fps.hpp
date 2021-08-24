//
// Created by z on 2021/8/22.
//

#pragma once

#include <vector>

#include "calc_sample.hpp"

namespace ssp {

class FPS final : public CalcSample<> {
 public:
  bool update(float &fps, CalcSample::TICK_TYPE tick = Sample::now().tick);

 private:
  float currentFPS = 0;
};

}



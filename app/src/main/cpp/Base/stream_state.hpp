//
// Created by z on 2021/8/22.
//

#pragma once

#include "fps.hpp"
#include "bitrate.hpp"

namespace ssp {

class StreamState {
 public:
  StreamState() = default;

  virtual ~StreamState() = default;

  void update(Sample sample = Sample::zero());

  uint64_t getTotalSize() const;

 protected:
  virtual void onStart();

  virtual void onStop();

  virtual void onCatchFps(float fps);

  virtual void onCatchBitrate(uint64_t bitrate);

  virtual void onHasFirstSampleEvent();

  virtual void onTimeOutEvent();

  virtual void onLostEvent();

};

}


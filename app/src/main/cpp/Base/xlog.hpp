//
// Created by z on 2021/8/25.
//

#pragma once
#include "Base/3rdparty/spdlog/spdlog.h"

namespace xlab::base {

class XLog : public spdlog::logger {
 public:
  using base = spdlog::logger;
  using base::base;

  template<bool Console>
  static std::shared_ptr<spdlog::logger> get(std::string logger_name = "", std::string path = "") {
	if (Console) {

	}

	auto log = spdlog::get(logger_name);
	return log;
  }

};

}
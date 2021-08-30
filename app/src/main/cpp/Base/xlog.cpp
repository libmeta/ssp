//
// Created by x on 2021/8/30.
//

#include "xlog.hpp"

#include "Base/3rdparty/spdlog/async.h"
#include "Base/3rdparty/spdlog/sinks/basic_file_sink.h"
#include "Base/3rdparty/spdlog/sinks/daily_file_sink.h"
#include "Base/3rdparty/spdlog/sinks/android_sink.h"
#include "Base/3rdparty/spdlog/fmt/ostr.h" // must be included
#include "Base/3rdparty/spdlog/sinks/stdout_sinks.h"

namespace xlab::base {

std::shared_ptr<spdlog::logger> CXLog::get() {
    std::lock_guard<std::mutex> Locker(LoggerLock);
    std::shared_ptr<spdlog::logger> log = spdlog::get(ConsoleLoggerName);
    if (log == nullptr) {
        log = CreateConsoleLog(ConsoleLoggerName);
    }
    return log;
}

std::shared_ptr<spdlog::logger> CXLog::get(std::string logger_name, std::string path) {
    std::lock_guard<std::mutex> Locker(LoggerLock);
    std::shared_ptr<spdlog::logger> log = spdlog::get(logger_name);
    if (log == nullptr) {
        log = CreateTextLog(logger_name, path);
    }
    return log;
}

std::shared_ptr<spdlog::logger> CXLog::CreateConsoleLog(std::string logger_name) {
#ifdef ANDROID
    auto console_logger = spdlog::android_logger_mt(logger_name, "sdk");
    console_logger->set_pattern("%v");
    console_logger->set_level(spdlog::level::info);
    console_logger->flush_on(spdlog::level::info);
    return console_logger;
#else
    auto console_logger = spdlog::stdout_logger_mt(logger_name);
    console_logger->set_pattern("[%L] [%H:%M:%S.%e] [thread %t] %v");
    console_logger->set_level(spdlog::level::info);
    console_logger->flush_on(spdlog::level::info);
    return console_logger;
#endif
};

///logger_name 不能重名
std::shared_ptr<spdlog::logger> CXLog::CreateTextLog(std::string logger_name, std::string path, spdlog::level::level_enum lvl) {
    spdlog::init_thread_pool(8192, 1); // 队列有 8k 个项目和 1 个支持线程。
    auto async_file = spdlog::daily_logger_mt<spdlog::async_factory>(logger_name, path, 2, 30);
    async_file->set_pattern("[%L] [%H:%M:%S.%e] [thread %t] %v");
    async_file->set_level(lvl);
    async_file->flush_on(lvl);
    return async_file;
}


}

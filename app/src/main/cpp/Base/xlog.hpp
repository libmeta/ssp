//
// Created by x on 2021/8/24.
//

#pragma once

#include "Base/3rdparty/spdlog/spdlog.h"
#include "Base/3rdparty/spdlog/async.h"
#include "Base/3rdparty/spdlog/sinks/basic_file_sink.h"
#include "Base/3rdparty/spdlog/sinks/daily_file_sink.h"
#include "Base/3rdparty/spdlog/sinks/android_sink.h"
#include "Base/3rdparty/spdlog/fmt/ostr.h" // must be included
#include "Base/3rdparty/spdlog/sinks/stdout_sinks.h"

namespace ssp::base {

class CXLog final {
    static inline std::mutex LoggerLock;
    static inline const char *ConsoleLoggerName = "console";
    static inline const char *TextLoggerName = "text";
    static inline std::string TextLoggerPath = "/sdcard/XLive/log/sdk/sdk.log";
public:
    using level = spdlog::level::level_enum;
    using spdlogger = spdlog::logger;

    ~CXLog() {
        spdlog::shutdown();
    }

    static void setTextPath(std::string path) {
        TextLoggerPath = path;
    }

    static CXLog &getInstance() {
        static CXLog inst;
        return inst;
    }

    static std::shared_ptr <spdlog::logger> console() {
        static auto console = xlab::base::CXLog::get();
        return console;
    }

    static std::shared_ptr <spdlog::logger> text() {
        static auto text = xlab::base::CXLog::get(TextLoggerName, TextLoggerPath);
        return text;
    }

    template<typename... Args>
    void log(spdlog::level::level_enum lvl, fmt::format_string<Args...> fmt, Args &&...args) {
        console()->log(lvl, fmt, std::forward<Args>(args)...);
        text()->log(lvl, fmt, std::forward<Args>(args)...);
    }

#define XLog ssp::base::CXLog::getInstance()
#define XLevel ssp::base::CXLog::level

#define t(fmt, ...) log(spdlog::level::trace,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define d(fmt, ...) log(spdlog::level::debug,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define i(fmt, ...) log(spdlog::level::info,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define w(fmt, ...) log(spdlog::level::warn,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define e(fmt, ...) log(spdlog::level::err,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define c(fmt, ...) log(spdlog::level::critical,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)

private:
    CXLog() {};

    CXLog(const CXLog &) = delete;

    CXLog &operator=(const CXLog &) = delete;

    static std::shared_ptr <spdlog::logger> get() {
        std::lock_guard <std::mutex> Locker(LoggerLock);
        std::shared_ptr <spdlog::logger> log = spdlog::get(ConsoleLoggerName);
        if (log == nullptr) {
            log = CreateConsoleLog(ConsoleLoggerName);
        }
        return log;
    }

    static std::shared_ptr <spdlog::logger> get(std::string logger_name, std::string path) {
        std::lock_guard <std::mutex> Locker(LoggerLock);
        std::shared_ptr <spdlog::logger> log = spdlog::get(logger_name);
        if (log == nullptr) {
            log = CreateTextLog(logger_name, path);
        }
        return log;
    }

    static std::shared_ptr <spdlog::logger> CreateConsoleLog(std::string logger_name) {
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
    static std::shared_ptr <spdlog::logger> CreateTextLog(std::string logger_name, std::string path, spdlog::level::level_enum level = spdlog::level::info) {
        spdlog::init_thread_pool(8192, 1); // 队列有 8k 个项目和 1 个支持线程。
        auto async_file = spdlog::daily_logger_mt<spdlog::async_factory>(logger_name, path, 2, 30);
        async_file->set_pattern("[%L] [%H:%M:%S.%e] [thread %t] %v");
        async_file->set_level(level);
        async_file->flush_on(level);
        return async_file;
    };

};

}

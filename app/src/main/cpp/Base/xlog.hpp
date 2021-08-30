//
// Created by x on 2021/8/24.
//

#pragma once

#include "Base/3rdparty/spdlog/spdlog.h"

namespace xlab::base {

class CXLog final {
    static inline std::mutex LoggerLock;
    static inline const char *ConsoleLoggerName = "console";
    static inline const char *TextLoggerName = "text";
    static inline std::string TextLoggerPath = "/sdcard/XLive/log/sdk/sdk.log";
public:
    using ELevel = spdlog::level::level_enum;
    using logger = spdlog::logger;

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

    static std::shared_ptr<spdlog::logger> console() {
        static auto console = xlab::base::CXLog::get();
        return console;
    }

    static std::shared_ptr<spdlog::logger> text() {
        static auto text = xlab::base::CXLog::get(TextLoggerName, TextLoggerPath);
        return text;
    }

    template<typename... Args>
    void log(spdlog::level::level_enum lvl, fmt::format_string<Args...> fmt, Args &&...args) {
        console()->log(lvl, fmt, std::forward<Args>(args)...);
        text()->log(lvl, fmt, std::forward<Args>(args)...);
    }

private:
    CXLog() = default;

    CXLog(const CXLog &) = delete;

    CXLog &operator=(const CXLog &) = delete;

    static std::shared_ptr<spdlog::logger> get();

    static std::shared_ptr<spdlog::logger> get(std::string logger_name, std::string path);

    static std::shared_ptr<spdlog::logger> CreateConsoleLog(std::string logger_name);

    ///logger_name 不能重名
    static std::shared_ptr<spdlog::logger> CreateTextLog(std::string logger_name,
                                                         std::string path,
                                                         spdlog::level::level_enum lvl = spdlog::level::info);

};
}

///打印指针用fmt::ptr()转换

#ifndef xlog
#define xlog xlab::base::CXLog::getInstance()
#endif

#ifndef XLevel
#define XLevel xlab::base::CXLog::ELevel
#endif

#ifndef xlogt
#define xlogt(fmt, ...) xlab::base::CXLog::getInstance().log(spdlog::level::trace,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif

#ifndef xlogd
#define xlogd(fmt, ...) xlab::base::CXLog::getInstance().log(spdlog::level::debug,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif

#ifndef xlogi
#define xlogi(fmt, ...) xlab::base::CXLog::getInstance().log(spdlog::level::info,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif

#ifndef xlogw
#define xlogw(fmt, ...) xlab::base::CXLog::getInstance().log(spdlog::level::warn,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif

#ifndef xloge
#define xloge(fmt, ...) xlab::base::CXLog::getInstance().log(spdlog::level::err,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif

#ifndef xlogc
#define xlogc(fmt, ...) xlab::base::CXLog::getInstance().log(spdlog::level::critical,"[{}:{}] [{}] : " fmt,basename(__FILE_NAME__), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif


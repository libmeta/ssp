//
// Created by x on 2021/8/27.
//

#pragma once

#include <sstream>

extern "C" {
#include <libavutil/log.h>
}

#ifdef __ANDROID__

#include <android/log.h>

#define FFLOG(type, fmt, ...)    __android_log_print(type, "FFLog", fmt, ##__VA_ARGS__)
#define FFLOG_D(fmt, ...)         FFLOG(ANDROID_LOG_DEBUG, fmt, ##__VA_ARGS__)
#define FFLOG_I(fmt, ...)         FFLOG(ANDROID_LOG_INFO, fmt, ##__VA_ARGS__)
#define FFLOG_E(fmt, ...)         FFLOG(ANDROID_LOG_ERROR, fmt, ##__VA_ARGS__)
#else
#define FFLOG(fmt, ...)      printf("\n[FFLog]" fmt"\n", ##__VA_ARGS__)
#define FFLOG_D(fmt, ...)    FFLOG("\n[D] [FFLog]" fmt"\n", ##__VA_ARGS__)
#define FFLOG_I(fmt, ...)    FFLOG("\n[I] [FFLog]" fmt"\n", ##__VA_ARGS__)
#define FFLOG_E(fmt, ...)    FFLOG("\n[E] [FFLog]" fmt"\n", ##__VA_ARGS__)
#endif

#include "Base/file_system.hpp"

namespace ssp::base::FFmpeg {

class FFLog {
    static inline FFLog *flog = nullptr;

    FFLog() {
        flog = this;
        av_log_set_level(AV_LOG_TRACE);
        av_log_set_callback(&FFLog::logOutput);
    }

    FFLog(const FFLog &) = delete;

    FFLog &operator=(const FFLog &) = delete;

    static void logOutput(void *ptr, int level, const char *fmt, va_list vaList) {
        std::ostringstream outStrStream;
        char line[1024];
        static int print_prefix = 1;
        va_list vaList2;
        va_copy(vaList2, vaList);
        av_log_format_line(ptr, level, fmt, vaList2, line, sizeof(line), &print_prefix);
        va_end(vaList2);
        switch (level) {
            case AV_LOG_DEBUG:
                outStrStream << " [AV_LOG_DEBUG] " << std::string(line) << std::endl;
                break;
            case AV_LOG_VERBOSE:
                outStrStream << " [AV_LOG_VERBOSE] " << std::string(line) << std::endl;
                break;
            case AV_LOG_INFO:
                outStrStream << " [AV_LOG_INFO] " << std::string(line) << std::endl;
                break;
            case AV_LOG_WARNING:
                outStrStream << " [AV_LOG_WARNING] " << std::string(line) << std::endl;
                break;
            case AV_LOG_ERROR:
                outStrStream << " [AV_LOG_ERROR] " << std::string(line) << std::endl;
                break;
            default:
                outStrStream << " [DEFAULT] " << std::string(line) << std::endl;
                break;
        }

        FFLOG_D("%s\n", outStrStream.str().c_str());

        if (flog->logFile != nullptr) {
            ::fwrite(outStrStream.str().c_str(), outStrStream.str().size(), 1, flog->logFile);
        }

        if (flog->logCallBack != nullptr) {
            flog->logCallBack(std::string(line), flog->logObject);
        }

    }

public:
    typedef void (*CallBack)(std::string msg, void *p_object);

    ~FFLog() {
        av_log_set_callback(nullptr);
        flog = nullptr;
        if (logFile != nullptr) {
            ::fclose(logFile);
            logFile = nullptr;
        }
    }

    static FFLog &getInstance() {
        static FFLog inst;
        return inst;
    }

    void setCallback(CallBack pFunction, void *pObject) {
        this->logCallBack = pFunction;
        this->logObject = pObject;
    }

    void setLevel(int level) {
        av_log_set_level(level);
    }

    void setLogDir(std::string logDir) {
        if (logFile != nullptr) {
            if (this->dir == logDir) {
                return;
            }

            ::fclose(logFile);
            logFile = nullptr;
        }

        ssp::base::FileSystem::MKDIR_P(logDir.c_str());
        logFile = ::fopen(std::string(logDir + "/" + logName).c_str(), "w");
        if (logFile == nullptr) {
            return;
        }

        this->dir = logDir;
    }

private:
    CallBack logCallBack = nullptr;
    void *logObject = nullptr;
    FILE *logFile = nullptr;
    std::string dir = ".";
    static inline const char *logName = "fflog.txt";
};

}

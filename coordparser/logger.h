//
// Created by Hiroki Teranishi on 3/2/17.
//

#ifndef COORDPARSER_LOGGER_H
#define COORDPARSER_LOGGER_H

#include <map>
#include "utility.h"

namespace coordparser {

class Logger {

public:
    enum class LogLevel {
        NONE    = 0b0000'0000'0000'0000'0000'0000,
        ERROR   = 0b0000'0000'0000'0000'0000'0001,
        WARNING = 0b0000'0000'0000'0000'0000'0010,
        NOTICE  = 0b0000'0000'0000'0000'0000'1000,
        INFO    = 0b0001'0000'0000'0000'0000'0000,
        DEBUG   = 0b0010'0000'0000'0000'0000'0000,
        TRACE   = 0b0100'0000'0000'0000'0000'0000,
        ALL     = 0b1111'1111'1111'1111'1111'1111,
    };

    template <typename ... Args>
    static void log(LogLevel logLevel, const char* format, const Args&... args);

    template <typename ... Args>
    static void error(const char* format, const Args&... args);

    template <typename ... Args>
    static void warning(const char* format, const Args&... args);

    template <typename ... Args>
    static void notice(const char* format, const Args&... args);

    template <typename ... Args>
    static void info(const char* format, const Args&... args);

    template <typename ... Args>
    static void debug(const char* format, const Args&... args);

    template <typename ... Args>
    static void trace(const char* format, const Args&... args);

protected:
    static const std::map<LogLevel, std::string> LABELS;
    std::string accessId;
    std::string accessTime;
    Logger() {};  // constructor


    static void _initialize();

    static void _finalize();

    static Logger& _getInstance();

    void _log(LogLevel logLevel, const char* message);
};

template<typename ... Args>
void Logger::log(LogLevel logLevel, const char* format, const Args&... args) {
    std::string message = utility::string::format(format, args...);
    _getInstance()._log(logLevel, message.c_str());
}

template<typename ... Args>
void Logger::error(const char* format, const Args&... args) {
    log(LogLevel::ERROR, format, args...);
}

template<typename ... Args>
void Logger::warning(const char* format, const Args&... args) {
    log(LogLevel::WARNING, format, args...);
}

template<typename ... Args>
void Logger::notice(const char* format, const Args&... args) {
    log(LogLevel::NOTICE, format, args...);
}

template<typename ... Args>
void Logger::info(const char* format, const Args&... args) {
    log(LogLevel::INFO, format, args...);
}

template<typename ... Args>
void Logger::debug(const char* format, const Args&... args) {
    log(LogLevel::DEBUG, format, args...);
}

template<typename ... Args>
void Logger::trace(const char* format, const Args&... args) {
    log(LogLevel::TRACE, format, args...);
}

}

#endif //COORDPARSER_LOGGER_H

//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_LOGGER_H_
#define COORDPARSER_LOGGER_H_

#include <map>
#include <string>
#include <iostream>

#include "coordparser/utility.h"

namespace coordparser {

namespace log {

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

static const std::map<LogLevel, std::string> labels = {
    {LogLevel::NONE, "none"},
    {LogLevel::ERROR, "error"},
    {LogLevel::WARNING, "warn"},
    {LogLevel::NOTICE, "notice"},
    {LogLevel::INFO, "info"},
    {LogLevel::DEBUG, "debug"},
    {LogLevel::TRACE, "trace"},
    {LogLevel::ALL, "all"},
};

static inline std::string label(LogLevel logLevel) {
  return labels.at(logLevel);
}

}  // namespace log

class Logger {
 public:
  ~Logger() {
    _finalize();
  }

  static Logger& getInstance() {
    static Logger instance;
    return instance;
  }

  template <typename ... Args>
  inline void log(log::LogLevel logLevel, const char* format,
                  const Args&... args) {
    if (!_should_log(logLevel)) {
      return;
    }
    _log(logLevel, utility::string::format(format, args...));
  }

  template <typename T>
  inline void log(log::LogLevel logLevel, const T& message) const {
    if (!_should_log(logLevel)) {
      return;
    }
    _log(logLevel, message);
  }

 protected:
  Logger() {
    verbose_ = true;
    format_ = "[%s]\t%s";
    file_ = "/Users/hiroki/work/coordparser/logs/test.log";
    _initialize();
  }

  void _initialize() {
    ofs_.open(file_, std::ofstream::out | std::ofstream::app);
    this->log(log::LogLevel::INFO, "logger._initialize() called.");
  }

  void _finalize() {
    this->log(log::LogLevel::INFO, "logger._initialize() called.");
    ofs_.close();
  }

  inline void _log(log::LogLevel logLevel,
                   const std::string& message) const {
    _log_raw(utility::string::format(format_.c_str(),
                                     log::label(logLevel), message));
  }

  inline void _log_raw(const std::string& message) const {
    ofs_ << message << std::endl;
    if (verbose_) {
      std::cout << message << std::endl;
    }
  }

  inline bool _should_log(log::LogLevel logLevel) const {
    return logLevel >= logLevel;
  }

  std::string accessId_;
  std::string accessTime_;
  log::LogLevel logLevel_;
  bool verbose_;
  std::string format_;
  std::string file_;

 private:
  std::ofstream ofs_;

  DISALLOW_COPY_AND_MOVE(Logger);
};

namespace log {

template <typename ... Args>
static void log(LogLevel logLevel, const char* format, const Args&... args) {
  Logger::getInstance().log(logLevel, format, args);
}

template <typename ... Args>
static void error(const char* format, const Args&... args) {
  log(LogLevel::ERROR, format, args...);
}

template <typename ... Args>
static void warning(const char* format, const Args&... args) {
  log(LogLevel::WARNING, format, args...);
}

template <typename ... Args>
static void notice(const char* format, const Args&... args) {
  log(LogLevel::NOTICE, format, args...);
}

template <typename ... Args>
static void info(const char* format, const Args&... args) {
  log(LogLevel::INFO, format, args...);
}

template <typename ... Args>
static void debug(const char* format, const Args&... args) {
  log(LogLevel::DEBUG, format, args...);
}

template <typename ... Args>
static void trace(const char* format, const Args&... args) {
  log(LogLevel::TRACE, format, args...);
}

template <typename T>
static void inline log(LogLevel logLevel, const T& obj) {
  Logger::getInstance().log(logLevel, obj);
}

template <typename T>
static inline void error(const T& obj) {
  log(LogLevel::ERROR, obj);
}

template <typename T>
static inline void warning(const T& obj) {
  log(LogLevel::WARNING, obj);
}

template <typename T>
static inline void notice(const T& obj) {
  log(LogLevel::NOTICE, obj);
}

template <typename T>
static inline void info(const T& obj) {
  log(LogLevel::INFO, obj);
}

template <typename T>
static inline void debug(const T& obj) {
  log(LogLevel::DEBUG, obj);
}

template <typename T>
static inline void trace(const T& obj) {
  log(LogLevel::TRACE, obj);
}

}  // namespace log

}  // namespace coordparser

#endif  // COORDPARSER_LOGGER_H_

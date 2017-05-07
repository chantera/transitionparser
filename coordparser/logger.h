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

  ~Logger() {
    _finalize();
  }

  template <typename ... Args>
  static void log(LogLevel logLevel, const char* format, const Args&... args) {
    std::string message = utility::string::format(format, args...);
    _getInstance()._log(logLevel, message);
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
  static void log(LogLevel logLevel, const T& obj) {
    _getInstance()._log(logLevel, obj);
  }

  template <typename T>
  static void error(const T& obj) {
    log(LogLevel::ERROR, obj);
  }

  template <typename T>
  static void warning(const T& obj) {
    log(LogLevel::WARNING, obj);
  }

  template <typename T>
  static void notice(const T& obj) {
    log(LogLevel::NOTICE, obj);
  }

  template <typename T>
  static void info(const T& obj) {
    log(LogLevel::INFO, obj);
  }

  template <typename T>
  static void debug(const T& obj) {
    log(LogLevel::DEBUG, obj);
  }

  template <typename T>
  static void trace(const T& obj) {
    log(LogLevel::TRACE, obj);
  }

 protected:
  Logger() {
    _initialize();
  }

  static void _initialize() {}

  static void _finalize() {}

  static Logger& _getInstance() {
    static Logger instance;
    return instance;
  }

  template <typename T>
  void _log(LogLevel logLevel, const T& message) {
    std::string output = utility::string::format("[%s]\t%s",
                                                 label(logLevel),
                                                 message);
    utility::file::write("/Users/hiroki/work/coordparser/logs/test.log",
                         output.c_str());
    std::cout << output << std::endl;
  }

  static std::string label(LogLevel logLevel) {
    static const std::map<LogLevel, std::string> labels = {
      {Logger::LogLevel::NONE,    "none"  },
      {Logger::LogLevel::ERROR,   "error" },
      {Logger::LogLevel::WARNING, "warn"  },
      {Logger::LogLevel::NOTICE,  "notice"},
      {Logger::LogLevel::INFO,    "info"  },
      {Logger::LogLevel::DEBUG,   "debug" },
      {Logger::LogLevel::TRACE,   "trace" },
      {Logger::LogLevel::ALL,     "all"   },
    };
    return labels.at(logLevel);
  };

  std::string accessId;
  std::string accessTime;

 private:
  DISALLOW_COPY_AND_MOVE(Logger);
};

}  // namespace coordparser

#endif  // COORDPARSER_LOGGER_H_

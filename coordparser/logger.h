//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_LOGGER_H_
#define COORDPARSER_LOGGER_H_

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/time.h>

#include <chrono>  // NOLINT(build/c++11)
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "coordparser/utility.h"

namespace coordparser {

/*
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

static inline std::string label(LogLevel log_level) {
  return labels.at(log_level);
}

class BaseHandler {
 public:
  BaseHandler() {}
  virtual ~BaseHandler() {}

  virtual void log(const std::string& message) = 0;

 private:
  DISALLOW_COPY_AND_MOVE(BaseHandler);
};

class FileHandler : public BaseHandler {
 public:
  FileHandler() {}
  explicit FileHandler(const std::string& filename) : file_(filename) {
    ofs_.open(file_, std::ofstream::out | std::ofstream::app);
  }
  ~FileHandler() {
    ofs_.close();
  }

  void log(const std::string& message) override {
    ofs_ << message << std::endl;
  }

 private:
  std::string file_;
  mutable std::ofstream ofs_;
};

class StdoutHandler : public BaseHandler {
 public:
  StdoutHandler() {}
  ~StdoutHandler() {}

  void log(const std::string& message) override {
    std::cout << message << std::endl;
  }
};

using handler_sptr = std::shared_ptr<BaseHandler>;

namespace internal {

static const LogLevel kDefaultLogLevel = LogLevel::INFO;
static const char* kDefaultFormat
    = "%(time)\t%(accessid)\t[%(level)]\t%(message)";
static const char* kDefaultTimeFormat = "%Y-%m-%d %H:%M:%S.%f %Z";

}  // namespace internal

}  // namespace log
 */

class Logger : public spdlog::logger {
 public:
  using clock = std::chrono::system_clock;

  template<class It>
  inline Logger(const std::string& logger_name, const It& begin, const It& end):
      access_id_(utility::hash::generate_uuid().substr(0, 6)),
      unique_id_("UNIQID"),
      access_time_(clock::now()),
      spdlog::logger(logger_name, begin, end) {
    _initialize();
  }

  inline Logger(const std::string& logger_name,
                spdlog::sinks_init_list sinks_list):
      Logger(logger_name, sinks_list.begin(), sinks_list.end()) {}

  inline Logger(const std::string& logger_name, spdlog::sink_ptr single_sink):
      Logger(logger_name, {single_sink}) {}

  inline ~Logger() {
    _finalize();
  }

 protected:
  void _initialize() {
    std::time_t t = clock::to_time_t(access_time_);
    std::tm tm;
    localtime_r(&t, &tm);
    time_format_ = utility::string::format("{} {:%Z}",
                                           "%Y-%m-%d %H:%M:%S.%f",
                                           tm);
    format_ = utility::string::format("{}\t{}\t[%l]\t%v",
                                      time_format_, access_id_);
    set_pattern(format_);
    info("LOG Start with ACCESSID=[{}] UNIQUEID=[{}] "
             "ACCESSTIME=[{:%Y-%m-%d %H:%M:%S.%f %Z}]",
         access_id_,
         unique_id_,
         tm);
  }

  void _finalize() {
    std::time_t t = clock::to_time_t(access_time_);
    std::tm tm;
    localtime_r(&t, &tm);
    const double elapsed_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            utility::date::now() - access_time_).count();
    info("LOG End with ACCESSID=[{}] UNIQUEID=[{}] "
             "ACCESSTIME=[{:%Y-%m-%d %H:%M:%S.%f %Z}] PROCESSTIME=[{:3.6f}]\n",
         access_id_,
         unique_id_,
         tm,
         (elapsed_time / 1000) / 1000);
  }

 private:
  const std::string access_id_;
  const std::string unique_id_;
  const clock::time_point access_time_;
  std::string time_format_;
  std::string format_;
};

namespace log {

namespace internal {

/*
class Registry {
 public:
  Registry() {
    createDefault();
  }
  ~Registry() {}
  static Registry& getInstance() {
    static Registry instance;
    return instance;
  }
  void registerLogger(const std::string& name,
                       const std::shared_ptr<Logger>& logger) {
    if (has(name)) {
      drop(name);
    }
    loggers_[name] = logger;
  }
  std::shared_ptr<Logger> create(const std::string& name) {
    std::string file = "/Users/hiroki/work/coordparser/logs/test.log";
    std::shared_ptr<Logger> logger(
        Logger::Builder()
            .setLogLevel(LogLevel::DEBUG)
            .setFormat(kDefaultFormat)
            // .addHandler(std::make_shared<FileHandler>(file))
            .addHandler(std::make_shared<StdoutHandler>())
            .build());
    registerLogger(name, logger);
    return logger;
  }
  std::shared_ptr<Logger> createDefault() {
    return create(defaultName());
  }
  std::shared_ptr<Logger> get(const std::string& name) {
    return loggers_.at(name);
  }
  std::shared_ptr<Logger> getDefault() {
    return get(defaultName());
  }
  bool has(const std::string &name) {
    return loggers_.find(name) != loggers_.end();
  }
  void drop(const std::string& name) {
    loggers_.erase(name);
  }

 protected:
  static const std::string& defaultName() {
    static const std::string kDefaultKey = "default";
    return kDefaultKey;
  }
  std::unordered_map<std::string, std::shared_ptr<Logger>> loggers_;

 private:
  DISALLOW_COPY_AND_MOVE(Registry);
};

static inline std::shared_ptr<Logger> get_default_logger() {
  return Registry::getInstance().getDefault();
}
 */

}  // namespace internal

inline std::shared_ptr<spdlog::logger> my_logger(
    const std::string& logger_name,
    const spdlog::filename_t& filename,
    bool truncate = false,
    bool stdout = true)
{
  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::move(
      std::make_shared<spdlog::sinks::simple_file_sink_st>(
          filename, truncate)));
  if (stdout) {
#ifdef _WIN32
    sinks.push_back(std::move(
        std::make_shared<spdlog::sinks::wincolor_stdout_sink_st>()));
#else
    sinks.push_back(std::move(
        std::make_shared<spdlog::sinks::ansicolor_sink>(
            spdlog::sinks::stdout_sink_st::instance())));
#endif
  }
  return std::make_shared<Logger>(logger_name, sinks.begin(), sinks.end());
}

namespace internal {

std::once_flag once;

void init() {
  const std::string file = "/Users/hiroki/work/coordparser/logs/test.log";
  spdlog::register_logger(my_logger("default", file, false, true));
}

inline std::shared_ptr<spdlog::logger> get_default_logger() {
  std::call_once(once, init);
  return spdlog::get("default");
}

}  // namespace internal

using LogLevel = spdlog::level::level_enum;

template <typename ... Args>
static void log(LogLevel log_level, const char* format, const Args&... args) {
  internal::get_default_logger()->log(log_level, format, args...);
}

template <typename ... Args>
static void error(const char* format, const Args&... args) {
  log(LogLevel::err, format, args...);
}

template <typename ... Args>
static void warning(const char* format, const Args&... args) {
  log(LogLevel::warn, format, args...);
}

template <typename ... Args>
static void info(const char* format, const Args&... args) {
  log(LogLevel::info, format, args...);
}

template <typename ... Args>
static void debug(const char* format, const Args&... args) {
  log(LogLevel::debug, format, args...);
}

template <typename ... Args>
static void trace(const char* format, const Args&... args) {
  log(LogLevel::trace, format, args...);
}

template <typename T>
static void inline log(LogLevel log_level, const T& obj) {
  internal::get_default_logger()->log(log_level, obj);
}

template <typename T>
static inline void error(const T& obj) {
  log(LogLevel::err, obj);
}

template <typename T>
static inline void warning(const T& obj) {
  log(LogLevel::warn, obj);
}

template <typename T>
static inline void info(const T& obj) {
  log(LogLevel::info, obj);
}

template <typename T>
static inline void debug(const T& obj) {
  log(LogLevel::debug, obj);
}

template <typename T>
static inline void trace(const T& obj) {
  log(LogLevel::trace, obj);
}

}  // namespace log

}  // namespace coordparser

#endif  // COORDPARSER_LOGGER_H_

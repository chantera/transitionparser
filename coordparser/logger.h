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
#include <utility>
#include <vector>

#include "coordparser/utility.h"

namespace coordparser {

namespace log {
  using LogLevel = spdlog::level::level_enum;
}  // namespace log

class Logger : public spdlog::logger {
 public:
  using clock = std::chrono::system_clock;

  template<class It>
  inline Logger(const std::string& logger_name, const It& begin, const It& end):
      access_id_(utility::hash::generate_uuid().substr(0, 6)),
      unique_id_("UNIQID"),
      access_time_(clock::now()),
      spdlog::logger(logger_name, begin, end) { _initialize(); }

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
    access_time_expr_ = utility::date::strftime_hr("%Y-%m-%d %H:%M:%S.%f %Z",
                                                   access_time_);
    std::time_t t = clock::to_time_t(access_time_);
    std::tm tm;
    localtime_r(&t, &tm);
    time_format_ = utility::string::format("{} {:%Z}",
                                           "%Y-%m-%d %H:%M:%S.%f",
                                           tm);
    format_ = utility::string::format("{}\t{}\t[%l]\t%v",
                                      time_format_, access_id_);
    set_pattern(format_);
    info("LOG Start with ACCESSID=[{}] UNIQUEID=[{}] ACCESSTIME=[{}]",
         access_id_,
         unique_id_,
         access_time_expr_);
  }

  void _finalize() {
    const double elapsed_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            utility::date::now() - access_time_).count();
    info("LOG End with ACCESSID=[{}] UNIQUEID=[{}] ACCESSTIME=[{}] "
             "PROCESSTIME=[{:3.6f}]\n",
         access_id_,
         unique_id_,
         access_time_expr_,
         (elapsed_time / 1000) / 1000);
  }

 private:
  const std::string access_id_;
  const std::string unique_id_;
  const clock::time_point access_time_;
  std::string access_time_expr_;
  std::string time_format_;
  std::string format_;
};

class AppLogger : public Logger {
 public:
  AppLogger() : Logger("app", sinks(true).begin(), sinks().end()) {
    set_level(log::LogLevel::trace);
  }
  static inline AppLogger& getInstance() {
    static AppLogger instance;
    return instance;
  }
  static void init(const spdlog::filename_t& filename,
                   log::LogLevel log_level = log::LogLevel::info,
                   log::LogLevel display_level = log::LogLevel::info) {
    if (sinks(false).size() > 0) {
      getInstance().warn("AppLogger has already been initialized.");
      return;
    }
    sinks().clear();
    auto file_sink_st =
        std::make_shared<spdlog::sinks::simple_file_sink_st>(filename);
    auto stdout_color_st =
#ifdef _WIN32
        std::make_shared<spdlog::sinks::wincolor_stdout_sink_st>();
#else
        std::make_shared<spdlog::sinks::ansicolor_sink>(
            spdlog::sinks::stdout_sink_st::instance());
#endif
    file_sink_st->set_level(log_level);
    stdout_color_st->set_level(display_level);
    sinks().push_back(std::move(file_sink_st));
    sinks().push_back(std::move(stdout_color_st));
  }

 protected:
  static std::vector<spdlog::sink_ptr>& sinks(bool init = false) {
    static std::vector<spdlog::sink_ptr> sinks_;
    if (init && sinks_.size() == 0) {
      sinks().push_back(spdlog::sinks::stdout_sink_st::instance());
    }
    return sinks_;
  }
};

namespace log {

template <typename ... Args>
static void log(LogLevel log_level, const char* format, const Args&... args) {
  AppLogger::getInstance().log(log_level, format, args...);
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
  AppLogger::getInstance().log(log_level, obj);
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

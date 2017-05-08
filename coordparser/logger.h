//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_LOGGER_H_
#define COORDPARSER_LOGGER_H_

#include <chrono>  // NOLINT(build/c++11)
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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

class Logger {
 public:
  class Builder {
   public:
    Logger* build() {
      return new Logger(handlers_.begin(), handlers_.end(), *this);
    }
    Builder& setLogLevel(const log::LogLevel log_level) {
      log_level_ = log_level;
      return *this;
    }
    Builder& setFormat(const std::string& format) {
      format_ = format;
      return *this;
    }
    Builder& setTimeFormat(const std::string& time_format) {
      time_format_ = time_format;
      return *this;
    }
    Builder& addHandler(const log::handler_sptr& handler) {
      handlers_.push_back(handler);
      return *this;
    }
    static const Builder& getDefault() {
      static const Builder kDefault{};
      return kDefault;
    }
    log::LogLevel log_level_ = log::internal::kDefaultLogLevel;
    std::string format_ = std::string(log::internal::kDefaultFormat);
    std::string time_format_ = std::string(log::internal::kDefaultTimeFormat);
    std::vector<log::handler_sptr> handlers_;
  };

  template <class InputIter>
  Logger(const InputIter& first, const InputIter& last,
         const Builder& builder = Builder::getDefault())
      : handlers_(first, last),
        log_level_(builder.log_level_),
        format_(builder.format_),
        time_format_(builder.time_format_),
        access_id_(utility::hash::generate_uuid().substr(0, 6)),
        unique_id_("UNIQID"),
        access_time_(utility::date::now()) {
    _initialize();
  }

  Logger(std::initializer_list<log::handler_sptr> handlers_list,
         const Builder& builder = Builder::getDefault())
      : Logger(handlers_list.begin(), handlers_list.end(), builder) {}

  explicit Logger(log::handler_sptr handler,
                  const Builder& builder = Builder::getDefault())
      : Logger({handler}, builder) {}

  ~Logger() {
    _finalize();
  }

  template <typename ... Args>
  inline void log(log::LogLevel log_level, const char* format,
                  const Args&... args) {
    if (!shouldLog(log_level)) {
      return;
    }
    _log(log_level, utility::string::format(format, args...));
  }

  template <typename T>
  inline void log(log::LogLevel log_level, const T& message) const {
    if (!shouldLog(log_level)) {
      return;
    }
    _log(log_level, message);
  }

  inline bool shouldLog(log::LogLevel log_level) const {
    return log_level_ >= log_level;
  }

 protected:
  void _initialize() {
    this->log(log::LogLevel::INFO,
              "LOG Start with ACCESSID=[%s] UNIQUEID=[%s] ACCESSTIME=[%s]",
              access_id_,
              unique_id_,
              utility::date::strftime_hr(time_format_, access_time_));
  }

  void _finalize() {
    double elapsed_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            utility::date::now() - access_time_).count();
    this->log(log::LogLevel::INFO,
              "LOG End with ACCESSID=[%s] UNIQUEID=[%s] ACCESSTIME=[%3.9f] "
                  "PROCESSTIME=[%s]\n",
              access_id_,
              "UNIQID",
              utility::date::strftime_hr(time_format_, access_time_),
              (elapsed_time / 1000) / 1000);
  }

  template <typename T>
  inline void _log(log::LogLevel log_level, const T& message) const {
    std::string output = format_;
    utility::string::replace(output, "%(time)",
                             utility::date::strftime_hr(time_format_));
    utility::string::replace(output, "%(accessid)", access_id_);
    utility::string::replace(output, "%(level)", log::label(log_level));
    utility::string::replace(output, "%(message)",
                             utility::string::to_string(message));
    _logRaw(output);
  }

  inline void _logRaw(const std::string& message) const {
    for (auto& handler : handlers_) {
      handler->log(message);
    }
  }

  std::vector<log::handler_sptr> handlers_;
  log::LogLevel log_level_;
  std::string format_;
  std::string time_format_;
  const std::string access_id_;
  const std::string unique_id_;
  const std::chrono::system_clock::time_point access_time_;

 private:
  Logger() = delete;
  DISALLOW_COPY_AND_MOVE(Logger);
};

namespace log {

namespace internal {

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
            .addHandler(std::make_shared<FileHandler>(file))
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

}  // namespace internal

template <typename ... Args>
static void log(LogLevel log_level, const char* format, const Args&... args) {
  internal::get_default_logger()->log(log_level, format, args...);
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
static void inline log(LogLevel log_level, const T& obj) {
  internal::get_default_logger()->log(log_level, obj);
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

//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_LOGGER_H_
#define COORDPARSER_LOGGER_H_

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

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

class BaseHandler {
 public:
  BaseHandler() {};
  virtual ~BaseHandler() {};

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

const LogLevel defaultLogLevel = LogLevel::INFO;
const std::string defaultFormat = "[%s]\t%s";

}  // namespace log

class Logger {
 public:
  class Builder {
   public:
    Builder& setLogLevel(log::LogLevel logLevel) {
      logLevel_ = logLevel;
      return *this;
    }
    Builder& setFormat(std::string& format) {
      format_ = format;
      return *this;
    }
    static const Builder& getDefault() {
      static const Builder kDefault{};
      return kDefault;
    }
    log::LogLevel logLevel_ = log::defaultLogLevel;
    std::string format_ = log::defaultFormat;
  };

  /*
  template <class InputIter>
  Logger(const InputIter& first, const InputIter& last,
         const Builder& builder = Builder::getDefault())
      : handlers_(first, last),
        logLevel_(builder.logLevel_),
        format_(builder.format_) {
    _initialize();
  }

  Logger(std::initializer_list<log::handler_sptr> handlers_list,
         const Builder& builder = Builder::getDefault())
      : Logger(handlers_list.begin(), handlers_list.end(), builder) {}
      */

  explicit Logger(log::handler_sptr handler,
                  const Builder& builder = Builder::getDefault())
      // : Logger({handler}, builder) {}
      : handlers_({handler}),
        logLevel_(builder.logLevel_),
        format_(builder.format_) {
    _initialize();
  }

  ~Logger() {
    _finalize();
  }

  template <typename ... Args>
  inline void log(log::LogLevel logLevel, const char* format,
                  const Args&... args) {
    if (!shouldLog(logLevel)) {
      return;
    }
    _log(logLevel, utility::string::format(format, args...));
  }

  template <typename T>
  inline void log(log::LogLevel logLevel, const T& message) const {
    if (!shouldLog(logLevel)) {
      return;
    }
    _log(logLevel, message);
  }

  inline bool shouldLog(log::LogLevel logLevel) const {
    return logLevel_ >= logLevel;
  }

 protected:
  void _initialize() {
    this->log(log::LogLevel::INFO, "logger._initialize() called.");
  }

  void _finalize() {
    this->log(log::LogLevel::INFO, "logger._initialize() called.");
  }

  template <typename T>
  inline void _log(log::LogLevel logLevel, const T& message) const {
    _logRaw(utility::string::format(format_.c_str(),
                                    log::label(logLevel), message));
  }

  inline void _logRaw(const std::string& message) const {
    for (auto& handler : handlers_) {
      handler->log(message);
    }
  }

  std::vector<log::handler_sptr> handlers_;
  std::string accessId_;
  std::string accessTime_;
  log::LogLevel logLevel_;
  std::string format_;
  std::string file_;

 private:
  Logger() = delete;
  DISALLOW_COPY_AND_MOVE(Logger);
};

namespace log {

namespace internal {

class Registry {
 public:
  Registry() {
    registerDefault();
  }
  ~Registry() {}
  static Registry& getInstance() {
    static Registry instance;
    return instance;
  }
  void registerLogger(const std::string& name,
                       const std::shared_ptr<Logger>& logger) {
    loggers_[name] = logger;
  }
  std::shared_ptr<Logger> get(const std::string& name) {
    return loggers_.at(name);
  }
  std::shared_ptr<Logger> getDefault() {
    return get(defaultName());
  }
 protected:
  static const std::string& defaultName() {
    static const std::string defaultKey = "default";
    return defaultKey;
  }
  std::unordered_map<std::string, std::shared_ptr<Logger>> loggers_;
 private:
  void registerDefault() {
    std::string file = "/Users/hiroki/work/coordparser/logs/test.log";
    // std::vector<handler_sptr> handlers;
    // handlers.push_back());
    // handlers.push_back(;

    /*
    std::shared_ptr<Logger> logger = std::make_shared<Logger>(
        {
            std::move(std::make_shared<FileHandler>(file)),
            std::move(std::make_shared<StdoutHandler>()),
        }
    );
     */
    std::shared_ptr<Logger> logger = std::make_shared<Logger>(
        std::move(std::make_shared<FileHandler>(file))
    );
    registerLogger(defaultName(), logger);
    //Builder builder
  }
  DISALLOW_COPY_AND_MOVE(Registry);
};

static inline std::shared_ptr<Logger> getDefaultLogger() {
  return Registry::getInstance().getDefault();
}

}  // namespace internal

template <typename ... Args>
static void log(LogLevel logLevel, const char* format, const Args&... args) {
  internal::getDefaultLogger()->log(logLevel, format, args...);
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
  internal::getDefaultLogger()->log(logLevel, obj);
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

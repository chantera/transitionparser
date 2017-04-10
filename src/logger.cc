//
// Created by Hiroki Teranishi on 3/2/17.
//

#include <iostream>
#include "logger.h"

namespace coordparser {

const std::map<Logger::LogLevel, std::string> Logger::LABELS =
        {
                {Logger::LogLevel::NONE,    "none"  },
                {Logger::LogLevel::ERROR,   "error" },
                {Logger::LogLevel::WARNING, "warn"  },
                {Logger::LogLevel::NOTICE,  "notice"},
                {Logger::LogLevel::INFO,    "info"  },
                {Logger::LogLevel::DEBUG,   "debug" },
                {Logger::LogLevel::TRACE,   "trace" },
                {Logger::LogLevel::ALL,     "all"   },
        };

void Logger::_initialize() {

}

void Logger::_finalize() {

}

Logger& Logger::_getInstance() {
    static Logger instance;
    return instance;
}

void Logger::_log(LogLevel logLevel, const char* message) {
    std::string output = utility::string::format("[%s]\t%s", Logger::LABELS.at(logLevel), message);
    utility::file::write("/Users/hiroki/work/coordparser/logs/test.log", output.c_str());
    std::cout << output << std::endl;
}

}
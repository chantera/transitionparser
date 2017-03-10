//
// Created by Hiroki Teranishi on 3/2/17.
//

#include "app.h"
#include "logger.h"

namespace coordparser {

App::App() {}

void App::train() {
    Logger::log(Logger::LogLevel::INFO, "Hello, World!");
    Logger::log(Logger::LogLevel::INFO, "int: %d", 10);
    Logger::info("float: %f", 2.3);
}

}
//
// Created by Hiroki Teranishi on 3/6/17.
//

#ifndef COORDPARSER_UTILITY_H
#define COORDPARSER_UTILITY_H

#include <boost/format.hpp>
#include <sstream>
#include <fstream>

namespace coordparser {

class StringUtils {

public:
    template <typename... Args>
    static std::string format(const char* format, const Args&... args) {
        boost::format boost_format(format);
        return StringUtils::format(boost_format, args...);
    }

    template <typename T, typename... Args>
    static std::string format(boost::format& format, const T& arg, const Args&... args) {
        format = format % arg;
        return StringUtils::format(format, args...);
    }

    static std::string format(boost::format& format) {
        std::stringstream stream;
        stream << format;
        return stream.str();
    }
};

class FileUtils {

public:
    static void write(const char* path, const char* data) {
        std::ofstream outFile(path, std::ios::app);
        outFile << data << std::endl;
    }

};

}

#endif //COORDPARSER_UTILITY_H

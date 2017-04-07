//
// Created by Hiroki Teranishi on 3/6/17.
//

#ifndef COORDPARSER_UTILITY_H
#define COORDPARSER_UTILITY_H

#include <boost/format.hpp>
#include <sstream>
#include <fstream>

namespace utility {

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

/*
namespace string {

template <typename... Args>
std::string format(const char* fmt, const Args&... args) {
    boost::format boost_format(fmt);
    return format(boost_format, args...);
}

template <typename T, typename... Args>
std::string format(boost::format& fmt, const T& arg, const Args&... args) {
    fmt = fmt % arg;
    return format(fmt, args...);
}

std::string format(boost::format& fmt) {
    std::stringstream stream;
    stream << fmt;
    return stream.str();
}
 */

/*
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::vector<std::string> split(const std::string& string, char delimiter) {
    std::istringstream stream(string);
    std::string field;
    std::vector<std::string> splits;
    while (std::getline(stream, field, delimiter)) {
        splits.push_back(field);
    }
    return splits;
}

}
*/

class FileUtils {

public:

static void write(const char* path, const char* data) {
    std::ofstream outFile(path, std::ios::app);
    outFile << data << std::endl;
}

};

/*
namespace file {

void write(const char* path, const char* data) {
    std::ofstream outFile(path, std::ios::app);
    outFile << data << std::endl;
}

}
 */

}

#endif //COORDPARSER_UTILITY_H

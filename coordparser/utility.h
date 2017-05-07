//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_UTILITY_H_
#define COORDPARSER_UTILITY_H_

#include <boost/format.hpp>

#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#define DEFAULT_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = default; \
  TypeName& operator=(const TypeName&) = default

#define DEFAULT_MOVE_AND_ASSIGN(TypeName) \
  TypeName(TypeName&&) = default; \
  TypeName& operator=(TypeName&&) = default

#define DEFAULT_COPY_AND_MOVE(TypeName) \
  DEFAULT_COPY_AND_ASSIGN(TypeName); \
  DEFAULT_MOVE_AND_ASSIGN(TypeName)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete; \
  TypeName& operator=(const TypeName&) = delete

#define DISALLOW_MOVE_AND_ASSIGN(TypeName) \
  TypeName(TypeName&&) = delete; \
  TypeName& operator=(TypeName&&) = delete

#define DISALLOW_COPY_AND_MOVE(TypeName) \
  DISALLOW_COPY_AND_ASSIGN(TypeName); \
  DISALLOW_MOVE_AND_ASSIGN(TypeName)

namespace utility {

namespace vector {

template <typename T>
static inline std::string join(const std::vector<T>& vector, char delimiter) {
  std::stringstream ss;
  if (!vector.empty()) {
    ss << vector[0];
    for (decltype(vector.size()) i = 1, c = vector.size(); i < c; ++i) {
      ss << delimiter;
      ss << vector[i];
    }
  }
  return ss.str();
}

}  // namespace vector

namespace string {

namespace internal {

static std::string format(const boost::format& fmt) {
  std::stringstream stream;
  stream << fmt;
  return stream.str();
}

template<typename T, typename... Args>
static std::string format(boost::format& fmt,  // NOLINT(runtime/references)
                          const std::vector<T>& arg, Args&& ... args) {
  fmt = fmt % vector::join(arg, ' ');
  return format(fmt, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
static std::string format(boost::format& fmt,  // NOLINT(runtime/references)
                          const T& arg, Args&& ... args) {
  fmt = fmt % arg;
  return format(fmt, std::forward<Args>(args)...);
}

}  // namespace internal

template<typename... Args>
std::string format(const char* fmt, Args&& ... args) {
  boost::format boost_format(fmt);
  return internal::format(boost_format, std::forward<Args>(args)...);
}

static inline void ltrim(std::string& s) {  // NOLINT(runtime/references)
  s.erase(s.begin(),
          std::find_if(
            s.begin(),
            s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

static inline void rtrim(std::string& s) {  // NOLINT(runtime/references)
  s.erase(std::find_if(
            s.rbegin(),
            s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
          s.end());
}

static inline void trim(std::string& s) {  // NOLINT(runtime/references)
  ltrim(s);
  rtrim(s);
}

static inline std::vector<std::string> split(const std::string& string,
                                             char delimiter) {
  std::istringstream stream(string);
  std::string field;
  std::vector<std::string> splits;
  while (std::getline(stream, field, delimiter)) {
    splits.push_back(field);
  }
  return splits;
}

}  // namespace string

namespace file {

static inline void write(const char* path, const char* data) {
  std::ofstream outFile(path, std::ios::app);
  outFile << data << std::endl;
}

}  // namespace file

}  // namespace utility

#endif  // COORDPARSER_UTILITY_H_

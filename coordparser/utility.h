//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#ifndef COORDPARSER_UTILITY_H_
#define COORDPARSER_UTILITY_H_

#include <boost/format.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <algorithm>
#include <ctime>
#include <chrono>  // NOLINT(build/c++11)
#include <fstream>
#include <functional>
#include <regex>  // NOLINT(build/c++11)
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);

#ifndef USE_INTERNAL_FMT
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>
#endif

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

#define FMT_OPERATOR(TypeName) \
  template<typename OStream> \
  friend OStream& operator<<(OStream& os, const TypeName& obj) { \
    std::stringstream ss; \
    ss << obj; \
    os << ss.str(); \
    return os; \
  }

#define COORDPARSER_ASSERT(expr, msg) { \
    if (!(expr)) { \
      std::ostringstream oss; \
      oss << msg; \
      throw std::runtime_error(oss.str()); \
    } \
  }

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

#ifdef USE_INTERNAL_FMT
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
static inline std::string format(const char* fmt, Args&& ... args) {
  boost::format boost_format(fmt);
  return internal::format(boost_format, std::forward<Args>(args)...);
}
#else
template<typename... Args>
static inline std::string format(const char* fmt, const Args& ... args) {
  return fmt::format(fmt, args...);
}
#endif

static inline void replace(std::string& s,  // NOLINT(runtime/references)
                           const std::string& target,
                           const std::string& replacement) {
  std::string::size_type pos = 0;
  while (pos = s.find(target, pos), pos != std::string::npos) {
    s.replace(pos, target.length(), replacement);
    pos += replacement.length();
  }
}

static inline void replace(std::string& s,  // NOLINT(runtime/references)
                           const std::string& target,
                           const std::string& replacement, int count) {
  std::string::size_type pos = 0;
  while (pos = s.find(target, pos), pos != std::string::npos && count > 0) {
    s.replace(pos, target.length(), replacement);
    pos += replacement.length();
    --count;
  }
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

static inline std::vector<std::string> split(const std::string& s,
                                             char delimiter) {
  std::vector<std::string> result;
  std::string::size_type i = 0;
  auto pos = s.find(delimiter);
  while (pos != std::string::npos) {
    result.push_back(s.substr(i, pos-i));
    i = ++pos;
    pos = s.find(delimiter, pos);
  }
  if (pos == std::string::npos)
    result.push_back(s.substr(i, s.length()));
  return result;
}

template <typename T>
static inline std::string to_string(const T& message) {
  std::stringstream ss;
  ss << message;
  return ss.str();
}

template <typename S, class UnaryOperator>
static inline S transform(const S& s, UnaryOperator op) {
  S temp(s);
  std::transform(temp.begin(), temp.end(), temp.begin(), op);
  return temp;
}

template <typename S, class UnaryOperator>
static inline void transform(S* s, UnaryOperator op) {
  std::transform(s->begin(), s->end(), s->begin(), op);
}

static inline std::string toupper(const std::string& string) {
  return transform(string, ::toupper);
}

static inline void toupper(std::string* string) {
  transform(string, ::toupper);
}

static inline std::string tolower(const std::string& string) {
  return transform(string, ::tolower);
}

static inline void tolower(std::string* string) {
  transform(string, ::tolower);
}

}  // namespace string

namespace file {

static inline void write(const char* path, const char* data) {
  std::ofstream outFile(path, std::ios::app);
  outFile << data << std::endl;
}

}  // namespace file

namespace hash {

static inline std::string generate_uuid() {
  return boost::uuids::to_string(
      boost::uuids::random_generator{}());  // NOLINT(whitespace/braces)
}

}  // namespace hash

namespace date {

using clock = std::chrono::system_clock;

static inline clock::time_point now() {
  return clock::now();
}

static inline std::string strftime(const std::string& format,
                                   const struct tm* timeptr) {
  char buffer[80];
  std::strftime(buffer, sizeof(buffer), format.c_str(), timeptr);
  return std::string(buffer);
}

static inline std::string strftime(const std::string& format,
                                   const time_t& time) {
  struct tm tm;
  localtime_r(&time, &tm);
  return date::strftime(format, &tm);
}

static inline std::string strftime(const std::string& format,
                                   const clock::time_point& tp) {
  return date::strftime(format, clock::to_time_t(tp));
}

static inline std::string strftime(const std::string& format) {
  return date::strftime(format, time(nullptr));
}

static inline std::string strftime_hr(const std::string& format,
                                      const clock::time_point& tp) {
  std::string new_format = format;
  const auto d = tp.time_since_epoch();
  const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(d);
  const auto microseconds
      = std::chrono::duration_cast<std::chrono::microseconds>(d - seconds);
  char str[7];
  snprintf(str, sizeof(str), "%06lld",
           (long long int) microseconds.count());  // NOLINT(runtime/int)
  string::replace(new_format, "%f", str);
  return date::strftime(new_format, tp);
}

static inline std::string strftime_hr(const std::string& format) {
  return date::strftime_hr(format, now());
}

}  // namespace date

class CmdArgs {
 public:
  CmdArgs(int argc, const char* argv[]) : argc_(argc), argv_(argv) {
    parse();
  }

  friend std::ostream& operator<<(std::ostream& os, const CmdArgs& args) {
    os << "params: ";
    int i;
    int size = args.params_.size();
    for (i = 0; i < size; ++i) {
      os << args.params_[i];
      if (i != size - 1) {
        os << ", ";
      }
    }
    os << ", options: ";
    size = args.option_keys_.size();
    for (i = 0; i < size; ++i) {
      auto& key = args.option_keys_[i];
      auto& value = args.options_.at(key);
      os << key << "=" << value;
      if (i != size - 1) {
        os << ", ";
      }
    }
    return os;
  }

  // regex
  //     ^-(-)?([^=\s]+)(?:[=\s](.*))?$        1   2       3
  // long name pattern
  //     --long1=value1      long1 => value1   "-" "long1" "value1"
  //     --long2 value2      long2 => value2   "-" "long2" "value1"
  //     --long3             long3 => ""       "-" "long3" ""
  // short name pattern
  //     -a=value4           a     => value4   ""  "a"     "value4"
  //     -b value5           b     => value5   ""  "b"     "value5"
  //     -c                  c     => ""       ""  "c"     ""
  //     -def=value6         d     => ""       ""  "def"   "value6"
  //                         e     => ""       ""  "def"   "value6"
  //                         f     => value6   ""  "def"   "value6"
  //     -ghi value7         g     => ""       ""  "ghi"   "value7"
  //                         h     => ""       ""  "ghi"   "value7"
  //                         i     => value7   ""  "ghi"   "value7"
  //     -jkl                j     => ""       ""  "jkl"   ""
  //                         k     => ""       ""  "jkl"   ""
  //                         l     => ""       ""  "jkl"   ""
  void parse() {
    if (argc_ < 2) {
      return;
    }
    std::regex re("^-(-)?([^=\\s]+)(?:[=\\s](.*))?$");
    std::cmatch matches;
    for (int i = 1; i < argc_; ++i) {
      if (!std::regex_match(argv_[i], matches, re)) {
        params_.emplace_back(argv_[i]);
        continue;
      }
      std::vector<std::string> names;
      if (matches[1] == '-') {
        names.emplace_back(matches[2]);
      } else {
        for (char& ch : matches[2].str()) {
          names.push_back(std::string({ch}));
        }
      }
      std::string name = std::move(names.back());
      names.pop_back();
      for (auto& n : names) {
        add_option(n, "");
      }
      if (matches[3].str() != "") {
        add_option(name, matches[3]);
      } else if (i + 1 < argc_) {
        const char* next = argv_[i + 1];
        if (next[0] != '-') {
          add_option(name, next);
          i++;
        } else {
          add_option(name, "");
        }
      } else {
        add_option(name, "");
      }
    }
  }

  unsigned getParamSize() {
    return static_cast<unsigned>(params_.size());
  }

  std::string getParam(unsigned index) {
    return params_.at(index);
  }

  std::string getParamOrDefault(unsigned index,
                                const std::string& defaultValue) {
    if (index > params_.size()) {
      return defaultValue;
    }
    return params_[index];
  }

  bool hasOption(const std::string& key) {
    return !(options_.find(key) == options_.end());
  }

  const std::unordered_map<std::string, std::string>& getOptions() {
    return options_;
  }

  std::string getOption(const std::string& key) {
    return options_.at(key);
  }

  std::string getOptionOrDefault(const std::string& key,
                                 const std::string& defaultValue) {
    if (!hasOption(key)) {
      return defaultValue;
    }
    return options_[key];
  }

  int size() {
    return argc_;
  }

 private:
  void add_option(const std::string& key, const std::string& value) {
    option_keys_.push_back(key);
    options_[key] = value;
  }

  const int argc_;
  const char** argv_;
  std::vector<std::string> params_;
  std::vector<std::string> option_keys_;
  std::unordered_map<std::string, std::string> options_;
};

}  // namespace utility

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  os << utility::vector::join(v, ' ');
  return os;
}

#endif  // COORDPARSER_UTILITY_H_

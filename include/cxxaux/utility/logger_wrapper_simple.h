// Copyright (c) 2024 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.
//
/// @file logger_wrapper_simple.h
/// @brief a simple logger implement with std::cout
///
/// `SimpleConsoleLogger` and `SimpleFileLogger` are provided
///
/// the marco `CXXAUX_ENABLE_CONSOLE_LOGGER_SIMPLE` and
/// `CXXAUX_ENABLE_FILE_LOGGER_SIMPLE` are used to precise control, default the
/// both is defined. Or
/// ```
/// #define CXXAUX_ENABLE_FILE_LOGGER_SIMPLE
/// #include <cxxaux/utility/logger_wrapper_simple.h>
/// ```

#ifndef CXXAUX_UTILITY_LOGGER_WRAPPER_SIMPLE_H
#define CXXAUX_UTILITY_LOGGER_WRAPPER_SIMPLE_H
#include "logger_wrapper.h"
#include <chrono>
#include <iomanip>
#include <memory>

#if !defined(CXXAUX_ENABLE_CONSOLE_LOGGER_SIMPLE) &&                           \
    !defined(CXXAUX_ENABLE_FILE_LOGGER_SIMPLE)
#define CXXAUX_ENABLE_CONSOLE_LOGGER_SIMPLE
#define CXXAUX_ENABLE_FILE_LOGGER_SIMPLE
#endif

#ifdef CXXAUX_ENABLE_CONSOLE_LOGGER_SIMPLE
#include <iostream>
struct __cxxaux_simplog_console_tag;
#endif

#ifdef CXXAUX_ENABLE_FILE_LOGGER_SIMPLE
#include <fstream>
struct __cxxaux_simplog_file_tag;
#endif

namespace cxxaux {

struct __cxxaux_logger_traits_simple_impl {
  template <typename logger_type, typename... Args>
  static void debug(logger_type&& logger, Args&&... args) {
    if (logger.cfg.lvl <= LogLevel::DEBUG)
      print(logger, "debug", std::forward<Args>(args)...);
  }

  template <typename logger_type, typename... Args>
  static void info(logger_type&& logger, Args&&... args) {
    if (logger.cfg.lvl <= LogLevel::INFO)
      print(logger, "info", std::forward<Args>(args)...);
  }

  template <typename logger_type, typename... Args>
  static void warn(logger_type&& logger, Args&&... args) {
    if (logger.cfg.lvl <= LogLevel::WARN)
      print(logger, "warn", std::forward<Args>(args)...);
  }

  template <typename logger_type, typename... Args>
  static void error(logger_type&& logger, Args&&... args) {
    if (logger.cfg.lvl <= LogLevel::ERROR)
      print(logger, "error", std::forward<Args>(args)...);
  }

  template <typename logger_type, typename... Args>
  [[noreturn]] static void fatal(logger_type&& logger, Args&&... args) {
    if (logger.cfg.lvl <= LogLevel::FATAL) {
      print(logger, "fatal", std::forward<Args>(args)...);
      logger.os.flush();
    }
    throw std::runtime_error("exception raised by logger fatal");
  }

private:
  template <typename logger_type, typename... Args>
  static void print(logger_type&& logger, const char* level, Args&&... args) {
    if ((logger.cfg.flags &
         static_cast<unsigned>(LogCfgFlag::FORMAT_WITH_DATE_TIME)) ||
        (logger.cfg.flags &
         static_cast<unsigned>(LogCfgFlag::FORMAT_WITH_TIME_ONLY))) {
      auto now = std::chrono::system_clock::now();

      logger.os << '[';
      std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
      std::tm local_tm = *std::localtime(&now_time_t);
      logger.os << std::put_time(
          &local_tm,
          (logger.cfg.flags &
           static_cast<unsigned>(LogCfgFlag::FORMAT_WITH_DATE_TIME)) > 0
              ? "%Y-%m-%d %H:%M:%S"
              : "%H:%M:%S");

      auto duration = now.time_since_epoch();
      auto millis =
          std::chrono::duration_cast<std::chrono::milliseconds>(duration)
              .count() %
          1000;
      logger.os << '.' << std::setw(3) << std::setfill('0') << millis;
      logger.os << "] ";
    }

    if (!logger.cfg.module_name.empty()) {
      logger.os << '[' << logger.cfg.module_name << "] ";
    }

    logger.os << '[' << level << "] ";
    (logger.os << ... << std::forward<Args>(args));
    logger.os << '\n';
  }
};

#ifdef CXXAUX_ENABLE_CONSOLE_LOGGER_SIMPLE
template <>
struct __cxxaux_logger_traits<__cxxaux_simplog_console_tag>
    : __cxxaux_logger_traits_simple_impl {
  struct logger_type {
    LoggerConfig cfg;
    std::ostream& os;
  };

  static logger_type __createLogger(const LoggerConfig& cfg = {}) {
    return {cfg, std::cout};
  }
};
#endif

#ifdef CXXAUX_ENABLE_FILE_LOGGER_SIMPLE
template <>
struct __cxxaux_logger_traits<__cxxaux_simplog_file_tag>
    : __cxxaux_logger_traits_simple_impl {
  struct logger_type {
    LoggerConfig cfg;
    std::ofstream os;
  };

  // for later init
  static logger_type __createLogger() { return {{lvl : LogLevel::SILENT}, {}}; }

  static logger_type __createLogger(const std::string& filePath,
                                    const LoggerConfig& cfg = {}) {
    return {cfg, std::ofstream(filePath)};
  }
};
#endif

#ifdef CXXAUX_ENABLE_CONSOLE_LOGGER_SIMPLE
#undef CXXAUX_ENABLE_CONSOLE_LOGGER_SIMPLE
/// @brief logger to console
using SimpleConsoleLogger = __LoggerWrapper<__cxxaux_simplog_console_tag>;
#endif

#ifdef CXXAUX_ENABLE_FILE_LOGGER_SIMPLE
#undef CXXAUX_ENABLE_FILE_LOGGER_SIMPLE
/// @brief logger to file
///
/// `SimpleFileLogger()` and then `reset(file_path, cfg = {})`
/// or `SimpleFileLogger(file_path, cfg = {})`
using SimpleFileLogger = __LoggerWrapper<__cxxaux_simplog_file_tag>;
#endif

} // namespace cxxaux
#endif

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

#ifndef CXXAUX_UTILITY_LOGGER_WRAPPER_SPDLOG_H
#define CXXAUX_UTILITY_LOGGER_WRAPPER_SPDLOG_H
#include "logger_wrapper.h"
#include <spdlog/spdlog.h>

#if !defined(CXXAUX_ENABLE_CONSOLE_LOGGER_SPDLOG) &&                           \
    !defined(CXXAUX_ENABLE_FILE_LOGGER_SPDLOG)
#define CXXAUX_ENABLE_CONSOLE_LOGGER_SPDLOG
#define CXXAUX_ENABLE_FILE_LOGGER_SPDLOG
#endif

#ifdef CXXAUX_ENABLE_CONSOLE_LOGGER_SPDLOG
#include <spdlog/sinks/stdout_color_sinks.h>
struct __cxxaux_spdlog_console_tag;
#endif

#ifdef CXXAUX_ENABLE_FILE_LOGGER_SPDLOG
#include <spdlog/sinks/basic_file_sink.h>
struct __cxxaux_spdlog_file_tag;
#endif

namespace cxxaux {

struct __cxxaux_logger_traits_spdlog_impl {
  static std::string logDummyName() {
    static unsigned uid = 0;
    return "__dummy" + std::to_string(uid++);
  }

  static spdlog::level::level_enum logLevel(const LoggerConfig& cfg) {
    switch (cfg.lvl) {
    case LogLevel::DEBUG:
      return spdlog::level::debug;
    case LogLevel::INFO:
      return spdlog::level::info;
    case LogLevel::WARN:
      return spdlog::level::warn;
    case LogLevel::ERROR:
      return spdlog::level::err;
    case LogLevel::FATAL:
      return spdlog::level::critical;
    case LogLevel::SILENT:
      return spdlog::level::off;
    }
    return spdlog::level::off;
  }

  static std::string logPattern(const LoggerConfig& cfg) {
    std::string buf;
    buf.reserve(64);
    if (cfg.flags & static_cast<unsigned>(LogCfgFlag::FORMAT_WITH_DATE_TIME)) {
      buf = "[%Y-%m-%d %H:%M:%S.%e]";
    } else if (cfg.flags &
               static_cast<unsigned>(LogCfgFlag::FORMAT_WITH_TIME_ONLY)) {
      buf = "[%H:%M:%S.%e]";
    }

    if (!cfg.module_name.empty()) {
      buf += " [%n]";
    }

    buf += " [%l] %v";
    return buf;
  }
};

#ifdef CXXAUX_ENABLE_CONSOLE_LOGGER_SPDLOG
template <>
struct __cxxaux_logger_traits<__cxxaux_spdlog_console_tag>
    : __cxxaux_logger_traits_spdlog_impl {
  using logger_type = std::shared_ptr<spdlog::logger>;

  static logger_type __createLogger(const LoggerConfig& cfg = {}) {
    if (cfg.module_name.empty()) {
      spdlog::set_pattern(logPattern(cfg));
      spdlog::set_level(logLevel(cfg));
      return nullptr;
    }

    auto logger = spdlog::stdout_color_mt(cfg.module_name);
    logger->set_pattern(logPattern(cfg));
    logger->set_level(logLevel(cfg));
    return logger;
  }

  template <typename FormatT, typename... Args>
  static void debug(std::shared_ptr<spdlog::logger> logger, FormatT&& fmt,
                    Args&&... args) {
    if (logger)
      logger->debug(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
    else
      spdlog::debug(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
  }

  template <typename FormatT, typename... Args>
  static void info(std::shared_ptr<spdlog::logger> logger, FormatT&& fmt,
                   Args&&... args) {
    if (logger)
      logger->info(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
    else
      spdlog::info(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
  }

  template <typename FormatT, typename... Args>
  static void warn(std::shared_ptr<spdlog::logger> logger, FormatT&& fmt,
                   Args&&... args) {
    if (logger)
      logger->warn(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
    else
      spdlog::warn(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
  }

  template <typename FormatT, typename... Args>
  static void error(std::shared_ptr<spdlog::logger> logger, FormatT&& fmt,
                    Args&&... args) {
    if (logger)
      logger->error(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
    else
      spdlog::error(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
  }

  template <typename FormatT, typename... Args>
  [[noreturn]] static void fatal(std::shared_ptr<spdlog::logger> logger,
                                 FormatT&& fmt, Args&&... args) {
    if (logger) {
      logger->critical(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
      logger->flush();
    } else {
      spdlog::critical(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
    }
    throw std::runtime_error("exception raised by logger fatal");
  }
};
#endif

#ifdef CXXAUX_ENABLE_FILE_LOGGER_SPDLOG
template <>
struct __cxxaux_logger_traits<__cxxaux_spdlog_file_tag>
    : __cxxaux_logger_traits_spdlog_impl {
  using logger_type = std::shared_ptr<spdlog::logger>;

  static logger_type __createLogger() { return nullptr; }

  static logger_type __createLogger(const std::string& filePath,
                                    const LoggerConfig& cfg = {}) {
    logger_type logger;
    if (cfg.module_name.empty()) {
      logger = spdlog::basic_logger_mt(logDummyName(), filePath, true);
    } else {
      logger = spdlog::basic_logger_mt(cfg.module_name, filePath, true);
    }
    logger->set_pattern(logPattern(cfg));
    logger->set_level(logLevel(cfg));
    return logger;
  }

  template <typename FormatT, typename... Args>
  static void debug(std::shared_ptr<spdlog::logger> logger, FormatT&& fmt,
                    Args&&... args) {
    if (logger) {
      logger->debug(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
    }
  }

  template <typename FormatT, typename... Args>
  static void info(std::shared_ptr<spdlog::logger> logger, FormatT&& fmt,
                   Args&&... args) {
    if (logger) {
      logger->info(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
    }
  }

  template <typename FormatT, typename... Args>
  static void warn(std::shared_ptr<spdlog::logger> logger, FormatT&& fmt,
                   Args&&... args) {
    if (logger) {
      logger->warn(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
    }
  }

  template <typename FormatT, typename... Args>
  static void error(std::shared_ptr<spdlog::logger> logger, FormatT&& fmt,
                    Args&&... args) {
    if (logger) {
      logger->error(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
    }
  }

  template <typename FormatT, typename... Args>
  [[noreturn]] static void fatal(std::shared_ptr<spdlog::logger> logger,
                                 FormatT&& fmt, Args&&... args) {
    if (logger) {
      logger->critical(fmt::v10::runtime(fmt), std::forward<Args>(args)...);
      logger->flush();
    }
    throw std::runtime_error("exception raised by logger fatal");
  }
};
#endif

#if defined(CXXAUX_ENABLE_CONSOLE_LOGGER_SPDLOG)
#undef CXXAUX_ENABLE_CONSOLE_LOGGER_SPDLOG
/// @brief logger to console
using SPDConsoleLogger = __LoggerWrapper<__cxxaux_spdlog_console_tag>;
#endif

#if defined(CXXAUX_ENABLE_FILE_LOGGER_SPDLOG)
#undef CXXAUX_ENABLE_FILE_LOGGER_SPDLOG
/// @brief logger to file
///
/// `SPDFileLogger()` and then `reset(file_path, cfg = {})`
/// or `SPDFileLogger(file_path, cfg = {})`
using SPDFileLogger = __LoggerWrapper<__cxxaux_spdlog_file_tag>;
#endif

} // namespace cxxaux
#endif

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
/// @file logger_wrapper.h
/// @brief a general logger api to uniform the different loggers

#ifndef CXXAUX_UTILITY_LOGGER_WRAPPER_H
#define CXXAUX_UTILITY_LOGGER_WRAPPER_H
#include <string>

namespace cxxaux {

/// @brief define the 5 general used logger level
enum class LogLevel : unsigned {
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL, //< fatal will log and then throw
  SILENT //< no log
};

/// @brief details for a logger with bit flag
enum class LogCfgFlag : unsigned {
  FORMAT_WITH_DATE_TIME = 1,
  FORMAT_WITH_TIME_ONLY = 1 << 1
};

/// @brief configuration with a logger
///
/// The logger format combines of multi parts, including
/// - date, time and milli seconds(optional, default time)
/// - logger level(default info)
/// - a project or moudle name to diff logger owner(optional)
struct LoggerConfig {
  std::string module_name;
  LogLevel lvl = LogLevel::INFO;
  unsigned flags = static_cast<unsigned>(LogCfgFlag::FORMAT_WITH_TIME_ONLY);
};

/// @brief traits to be impled by the concrete logger
///
/// // a member type of the logger for the Loggerwrapper to hold it,
/// // it must be RAII as no delete logger api is designed
/// using logger_type;
///
/// // logger create api, usually the console logger just has a LoggerConfig
/// // and the file logger has an aditional file path.
/// // it can be more of those, just as the specific logger defined and the
/// // user called
/// static logger_type __createLogger(); // default is a must for later init
/// static logger_type __createLogger(const LoggerConfig& cfg = {});
/// static logger_type __createLogger(const std::string& filePath,
///                                 const LoggerConfig& cfg = {});
///
/// // 5 level logger api should be defined as the enum LoggerLevel
/// // it can be ostream or format like as the specific logger defines
/// template <typename logger_type, typename... Args>
/// static void debug(logger_type&& logger, Args&&... args);
/// template <typename logger_type, typename... Args>
/// static void info(logger_type&& logger, Args&&... args);
/// template <typename logger_type, typename... Args>
/// static void warn(logger_type&& logger, Args&&... args);
/// template <typename logger_type, typename... Args>
/// static void error(logger_type&& logger, Args&&... args);
/// template <typename logger_type, typename... Args>
/// static [[noreturn]] void fatal(logger_type&& logger, Args&&... args);
///
/// @tparam an unique logger tag
template <typename>
struct __cxxaux_logger_traits;

template <typename LoggerTag>
class __LoggerWrapper {
  /// @brief a RAII type holding the concrete logger
  using logger_type = typename __cxxaux_logger_traits<LoggerTag>::logger_type;

public:
  /// @brief generate the specific logger
  /// @tparam ...Args types of the params passed to traits::createLogger
  /// @param ...args params passed to traits::createLogger
  template <typename... Args>
  __LoggerWrapper(Args&&... args)
      : logger(__cxxaux_logger_traits<LoggerTag>::__createLogger(
            std::forward<Args>(args)...)) {}

  /// @brief used for the default constructor __LoggerWrapper as a later config
  /// @tparam ...Args types of the params passed to traits::createLogger
  /// @param ...args params passed to traits::createLogger
  template <typename... Args>
  void reset(Args&&... args) {
    static_assert(std::is_move_assignable_v<logger_type>);
    logger = __cxxaux_logger_traits<LoggerTag>::__createLogger(
        std::forward<Args>(args)...);
  }

  template <typename... Args>
  void debug(Args&&... args) {
    __cxxaux_logger_traits<LoggerTag>::debug(logger,
                                             std::forward<Args>(args)...);
  }

  template <typename... Args>
  void info(Args&&... args) {
    __cxxaux_logger_traits<LoggerTag>::info(logger,
                                            std::forward<Args>(args)...);
  }

  template <typename... Args>
  void warn(Args&&... args) {
    __cxxaux_logger_traits<LoggerTag>::warn(logger,
                                            std::forward<Args>(args)...);
  }

  template <typename... Args>
  void error(Args&&... args) {
    __cxxaux_logger_traits<LoggerTag>::error(logger,
                                             std::forward<Args>(args)...);
  }

  template <typename... Args>
  [[noreturn]] void fatal(Args&&... args) {
    __cxxaux_logger_traits<LoggerTag>::fatal(logger,
                                             std::forward<Args>(args)...);
    throw;
  }

private:
  logger_type logger;
};

} // namespace cxxaux
#endif

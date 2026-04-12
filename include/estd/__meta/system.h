// Copyright (c) 2023 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
//             http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef ESTD___META_SYSTEM_H
#define ESTD___META_SYSTEM_H
#include "operators.h"
#include <type_traits>

namespace es {

/**
 * @defgroup system_traits System Detection Traits
 * @brief Compile-time detection of operating system and platform
 * @{
 */

/// @brief Type trait to check if the platform is Windows (any version)
#ifdef __WIN32
struct is_win : std::true_type {};
#else
struct is_win : std::false_type {};
#endif

/// @brief Helper variable for is_win
constexpr bool is_win_v = is_win::value;

/// @brief Type trait to check if the platform is Windows 64-bit
#ifdef __WIN64
struct is_win64 : std::true_type {};
#else
struct is_win64 : std::false_type {};
#endif

/// @brief Helper variable for is_win64
constexpr bool is_win64_v = is_win64::value;

/// @brief Type trait to check if the platform is Windows 32-bit
struct is_win32 : meta_and_t<is_win, meta_not<is_win64>> {};

/// @brief Helper variable for is_win32
constexpr bool is_win32_v = is_win32::value;

/// @brief Type trait to check if the platform is Cygwin
#ifdef __CYGWIN__
struct is_cygwin : std::true_type {};
#else
struct is_cygwin : std::false_type {};
#endif

/// @brief Helper variable for is_cygwin
constexpr bool is_cygwin_v = is_cygwin::value;

/// @brief Type trait to check if the platform is Unix (Linux, BSD, macOS)
#ifdef __unix__
struct is_unix : std::true_type {};
#else
struct is_unix : std::false_type {};
#endif

/// @brief Helper variable for is_unix
constexpr bool is_unix_v = is_unix::value;

/// @brief Type trait to check if the platform is Linux
#ifdef __linux__
struct is_linux : std::true_type {};
#else
struct is_linux : std::false_type {};
#endif

/// @brief Helper variable for is_linux
constexpr bool is_linux_v = is_linux::value;

/// @brief Type trait to check if the platform is FreeBSD
#ifdef __FreeBSD__
struct is_free_bsd : std::true_type {};
#else
struct is_free_bsd : std::false_type {};
#endif

/// @brief Helper variable for is_free_bsd
constexpr bool is_free_bsd_v = is_free_bsd::value;

/// @brief Type trait to check if the platform is macOS
#if defined(__APPLE__)
struct is_macos : std::true_type {};
#else
struct is_macos : std::false_type {};
#endif

/// @brief Helper variable for is_macos
constexpr bool is_macos_v = is_macos::value;

/// @brief Type trait to check if the platform is Android
#ifdef __ANDROID__
struct is_android : std::true_type {};
#else
struct is_android : std::false_type {};
#endif

/// @brief Helper variable for is_android
constexpr bool is_android_v = is_android::value;

/** @} */

} // namespace es
#endif // ESTD___META_SYSTEM_H

#ifndef ESTD_SYSTEM_H
#define ESTD_SYSTEM_H
#include <type_traits>
#include "meta.h"

namespace estd {

/// win
#ifdef __WIN32
struct is_win : std::true_type {};
#else
struct is_win : std::false_type {};
#endif
constexpr bool is_win_v = is_win::value;

#ifdef __WIN64
struct is_win64 : std::true_type {};
#else
struct is_win64 : std::false_type {};
#endif
constexpr bool is_win64_v = is_win64::value;

struct is_win32 : meta_and_t<is_win, meta_not<is_win64>> {};
constexpr bool is_win32_v = is_win32::value;

#ifdef __CYGWIN__
struct is_cygwin : std::true_type {};
#else
struct is_cygwin : std::false_type {};
#endif
constexpr bool is_cygwin_v = is_cygwin::value;

/// unix(linux, *BSD, Mac OS X)
#ifdef __unix__
struct is_unix : std::true_type {};
#else
struct is_unix : std::false_type {};
#endif
constexpr bool is_unix_v = is_unix::value;

/// linux
#ifdef __linux__
struct is_linux : std::true_type {};
#else
struct is_linux : std::false_type {};
#endif
constexpr bool is_linux_v = is_linux::value;

/// FreeBSD
#ifdef __FreeBSD__
struct is_free_bsd : std::true_type {};
#else
struct is_free_bsd : std::false_type {};
#endif
constexpr bool is_free_bsd_v = is_free_bsd::value;

/// Mac OS X
#if defined(__APPLE__) && defined(__ARCH__)
struct is_macos : std::true_type {};
#else
struct is_macos : std::false_type {};
#endif
constexpr bool is_macos_v = is_macos::value;

/// Android
#ifdef __ANDROID__
struct is_android : std::true_type {};
#else
struct is_android : std::false_type {};
#endif
constexpr bool is_android_v = is_android::value;

}
#endif //ESTD_SYSTEM_H

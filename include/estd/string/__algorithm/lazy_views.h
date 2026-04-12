// Copyright (c) 2026 guyuemeng
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

#ifndef ESTD_STRING___ALGORITHM_LAZY_VIEWS_H
#define ESTD_STRING___ALGORITHM_LAZY_VIEWS_H

#include <cctype>
#include <cstring>
#include <estd/__ranges/range_adaptor_closure.h>
#include <estd/__ranges/subrange.h>
#include <estd/__ranges/transform_view.h>

namespace es { namespace string { namespace views {

/**
 * @brief Base template for string view adapters.
 *
 * This class template provides a foundation for creating string view adaptors
 * that can work with various string representations (ranges, C-strings, char
 * arrays). It inherits from range_adaptor_closure to enable pipe syntax and
 * composition.
 *
 * Key features:
 * - Works with ranges (via range_adaptor_closure)
 * - Works with C-strings (const char*)
 * - Works with char arrays (both const and non-const)
 * - Supports pipe syntax and composition
 *
 * @tparam Derived The derived closure type (CRTP pattern)
 * @tparam Func The function type that performs the actual transformation
 *
 * Example usage:
 * @code
 * // Define a custom string adapter
 * struct my_transform_range_closure {
 *     template<typename Range>
 *     constexpr auto operator()(Range&& r) const {
 *         // Transform the range
 *     }
 * };
 *
 * struct my_transform_closure
 *     : string_adapter_closure<my_transform_closure,
 * my_transform_range_closure> {};
 *
 * constexpr inline my_transform_closure my_transform;
 *
 * // Use with different string types
 * std::string str = "hello";
 * auto result1 = str | my_transform;  // Range
 * auto result2 = "hello" | my_transform;  // C-string
 * char arr[] = "hello";
 * auto result3 = arr | my_transform;  // Char array
 * @endcode
 */
template <typename Derived, typename Func>
struct string_adapter_closure : es::ranges::range_adaptor_closure<Derived> {
  /**
   * @brief Applies the transformation to a range.
   *
   * @tparam Range The range type
   * @param r The range to transform
   * @return Transformed range
   */
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return Func{}(std::forward<Range>(r));
  }

  /**
   * @brief Applies the transformation to a C-string.
   *
   * Creates a subrange from the C-string and applies the transformation.
   *
   * @param str Null-terminated C-string
   * @return Transformed range
   */
  constexpr auto operator()(const char* str) const {
    return operator()(es::ranges::subrange(str, str + std::strlen(str)));
  }

  /**
   * @brief Applies the transformation to a const char array.
   *
   * @tparam N Array size
   * @param arr The char array
   * @return Transformed range
   */
  template <size_t N>
  constexpr auto operator()(const char (&arr)[N]) const {
    return operator()(es::ranges::subrange(arr, arr + N));
  }

  /**
   * @brief Applies the transformation to a non-const C-string.
   *
   * @param str Null-terminated C-string
   * @return Transformed range
   */
  constexpr auto operator()(char* str) const {
    return operator()(es::ranges::subrange(str, str + std::strlen(str)));
  }

  /**
   * @brief Applies the transformation to a non-const char array.
   *
   * @tparam N Array size
   * @param arr The char array
   * @return Transformed range
   */
  template <size_t N>
  constexpr auto operator()(char (&arr)[N]) const {
    return operator()(es::ranges::subrange(arr, arr + N));
  }
};

/**
 * @brief Internal closure for to_upper transformation.
 */
struct to_upper_range_closure {
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return es::ranges::transform([](char c) {
      return static_cast<char>(std::toupper(c));
    })(std::forward<Range>(r));
  }
};

/**
 * @brief View adapter that converts characters to uppercase.
 *
 * Transforms each character in a string to its uppercase equivalent
 * using std::toupper. This is a lazy view that applies the transformation
 * on-the-fly during iteration.
 *
 * Example usage:
 * @code
 * std::string str = "hello world";
 * auto upper = str | to_upper;
 * // upper contains: "HELLO WORLD"
 *
 * // Works with C-strings
 * auto upper2 = "hello" | to_upper;
 *
 * // Works with char arrays
 * char arr[] = "hello";
 * auto upper3 = arr | to_upper;
 * @endcode
 */
struct to_upper_closure
    : string_adapter_closure<to_upper_closure, to_upper_range_closure> {};

/**
 * @brief Global instance of to_upper view adapter.
 */
constexpr inline to_upper_closure to_upper;

/**
 * @brief Internal closure for to_lower transformation.
 */
struct to_lower_range_closure {
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return es::ranges::transform([](char c) {
      return static_cast<char>(std::tolower(c));
    })(std::forward<Range>(r));
  }
};

/**
 * @brief View adapter that converts characters to lowercase.
 *
 * Transforms each character in a string to its lowercase equivalent
 * using std::tolower. This is a lazy view that applies the transformation
 * on-the-fly during iteration.
 *
 * Example usage:
 * @code
 * std::string str = "HELLO WORLD";
 * auto lower = str | to_lower;
 * // lower contains: "hello world"
 *
 * // Works with C-strings
 * auto lower2 = "HELLO" | to_lower;
 *
 * // Works with char arrays
 * char arr[] = "HELLO";
 * auto lower3 = arr | to_lower;
 * @endcode
 */
struct to_lower_closure
    : string_adapter_closure<to_lower_closure, to_lower_range_closure> {};

/**
 * @brief Global instance of to_lower view adapter.
 */
constexpr inline to_lower_closure to_lower;

/**
 * @brief Internal closure for dot_to_underline transformation.
 */
struct dot_to_underline_range_closure {
  template <typename Range>
  constexpr auto operator()(Range&& r) const {
    return es::ranges::transform([](char c) { return c == '.' ? '_' : c; })(
        std::forward<Range>(r));
  }
};

/**
 * @brief View adapter that replaces dots with underscores.
 *
 * Transforms each dot ('.') character in a string to an underscore ('_').
 * This is useful for converting identifiers that use dot notation to
 * underscore notation. This is a lazy view that applies the transformation
 * on-the-fly during iteration.
 *
 * Example usage:
 * @code
 * std::string str = "namespace.class.method";
 * auto result = str | dot_to_underline;
 * // result contains: "namespace_class_method"
 *
 * // Works with C-strings
 * auto result2 = "a.b.c" | dot_to_underline;
 *
 * // Works with char arrays
 * char arr[] = "config.value";
 * auto result3 = arr | dot_to_underline;
 * @endcode
 */
struct dot_to_underline_closure
    : string_adapter_closure<dot_to_underline_closure,
                             dot_to_underline_range_closure> {};

/**
 * @brief Global instance of dot_to_underline view adapter.
 */
constexpr inline dot_to_underline_closure dot_to_underline;

}}} // namespace es::string::views

#endif // ESTD_STRING___ALGORITHM_LAZY_VIEWS_H

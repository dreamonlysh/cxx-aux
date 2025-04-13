// Copyright (c) 2025 guyuemeng
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

#ifndef ESTD_STRING___ALGORITHM_REPLACE_H
#define ESTD_STRING___ALGORITHM_REPLACE_H
#include "type_traits_string.h"
#include <algorithm>

namespace es { namespace string {

namespace __impl_replace {
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void adjust_range_to_be_replaced(StringLike& s,
                                           typename StringLike::size_type pos,
                                           typename StringLike::size_type count,
                                           typename StringLike::size_type n) {
  if (pos >= s.size()) {
    throw std::out_of_range("replace pos out of range");
  }

  count = std::min(count, s.size() - pos);
  auto size_before_replace = s.size();
  auto size_after_replace = size_before_replace - count + n;
  if (count < n) {
    if constexpr (__impl_type_traits::has_member_resize_and_overwrite_v<
                      StringLike>) {
      s.resize_and_overwrite(size_after_replace, [&](auto* data, std::size_t) {
        std::copy_backward(data + pos + count, data + size_before_replace,
                           data + size_after_replace);
        return size_after_replace;
      });
    } else {
      s.resize(size_after_replace);
      std::copy_backward(s.data() + pos + count, s.data() + size_before_replace,
                         s.data() + size_after_replace);
    }
  } else if (count > n) {
    std::copy(s.data() + pos + count, s.data() + size_before_replace,
              s.data() + pos + n);
    s.resize(size_after_replace);
  }
}
} // namespace __impl_replace

/// @brief Replace the range [pos, pos + count) in string with the range [p, p +
/// n)
/// @tparam StringLike std::string like type
/// @param s string has range to be replaceed
/// @param pos start position to be replaced, throw std::out_of_range if pos is
/// out of range
/// @param count number of characters to be replaced
/// @param p begin of the string used to replace
/// @param n number of characters in p to be used to replace
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void replace(StringLike& s, typename StringLike::size_type pos,
                       typename StringLike::size_type count,
                       const typename StringLike::value_type* p,
                       typename StringLike::size_type n) {
  __impl_replace::adjust_range_to_be_replaced(s, pos, count, n);
  std::copy(p, p + n, s.data() + pos);
}

/// @brief Replace the range [pos, pos + count) in string with the a cstring
/// @tparam StringLike std::string like type
/// @param s string has range to be replaceed
/// @param pos start position to be replaced, throw std::out_of_range if pos is
/// out of range
/// @param count number of characters to be replaced
/// @param p cstring used to replace
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void replace(StringLike& s, typename StringLike::size_type pos,
                       typename StringLike::size_type count,
                       const typename StringLike::value_type* p) {
  replace(s, pos, count, p, strlen(p));
}

/// @brief Replace the range [pos, pos + count) in string with the string view
/// @tparam StringLike std::string like type
/// @tparam StringViewLike std::string_view like type
/// @param s string has range to be replaceed
/// @param pos start position to be replaced, throw std::out_of_range if pos is
/// out of range
/// @param count number of characters to be replaced
/// @param sv string view used to replace
template <typename StringLike, typename StringViewLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike> &&
              __impl_type_traits::is_string_view_like_v<StringViewLike>>>
constexpr void replace(StringLike& s, typename StringLike::size_type pos,
                       typename StringLike::size_type count,
                       const StringViewLike& sv) {
  replace(s, pos, count, sv.data(), sv.size());
}

/// @brief Replace the range [pos, pos + count) in string with n characters c
/// @tparam StringLike std::string like type
/// @param s string has range to be replaceed
/// @param pos start position to be replaced, throw std::out_of_range if pos is
/// out of range
/// @param count number of characters to be replaced
/// @param n number of characters to be used to replace
/// @param c character used to replace
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void replace(StringLike& s, typename StringLike::size_type pos,
                       typename StringLike::size_type count,
                       typename StringLike::size_type n,
                       typename StringLike::value_type c) {
  __impl_replace::adjust_range_to_be_replaced(s, pos, count, n);
  std::fill(s.data() + pos, s.data() + pos + n, c);
}

/// @brief Replace the range [pos, pos + count) in string with a initializer
/// list
/// @tparam StringLike std::string like type
/// @param s string has range to be replaceed
/// @param pos start position to be replaced, throw std::out_of_range if pos is
/// out of range
/// @param count number of characters to be replaced
/// @param il initializer list used to replace
template <typename StringLike,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void
replace(StringLike& s, typename StringLike::size_type pos,
        typename StringLike::size_type count,
        std::initializer_list<typename StringLike::value_type> il) {
  replace(s, pos, count, il.begin(), il.size());
}

/// @brief Replace the range [pos, pos + count) in string with the range [first,
/// last)
/// @tparam StringLike std::string like type
/// @param s string has range to be replaceed
/// @param pos start position to be replaced, throw std::out_of_range if pos is
/// out of range
/// @param count number of characters to be replaced
/// @param first range begin to be used to replace
/// @param last range end to be used to replace
template <typename StringLike, typename InputIterator,
          typename = std::enable_if_t<
              __impl_type_traits::is_string_like_v<StringLike>>>
constexpr void replace(StringLike& s, typename StringLike::size_type pos,
                       typename StringLike::size_type count,
                       InputIterator first, InputIterator last) {
  __impl_replace::adjust_range_to_be_replaced(s, pos, count,
                                              std::distance(first, last));
  std::copy(first, last, s.data() + pos);
}

}} // namespace es::string
#endif

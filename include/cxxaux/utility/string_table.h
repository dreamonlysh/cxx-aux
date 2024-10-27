// Copyright (c) 2024 guyuemeng
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

#ifndef CXXAUX_UTILITY_STRING_TABLE_H
#define CXXAUX_UTILITY_STRING_TABLE_H
#include <deque>
#include <mutex>
#include <string>
#include <string_view>

namespace cxxaux {

template <template <typename> typename SetT, typename T = void>
class StringTable {
public:
  static std::string_view add(std::string_view s) {
    StringTable& tbl = instance();

    static std::mutex mut;
    std::lock_guard lock(mut);

    auto it = tbl.strtbl_.find(s);
    if (it != tbl.strtbl_.end())
      return *it;

    tbl.storage_.push_back(std::string(s));
    tbl.strtbl_.insert(tbl.storage_.back());
    return tbl.storage_.back();
  }

private:
  static StringTable& instance() {
    static StringTable singleton;
    return singleton;
  }

  StringTable() = default;
  ~StringTable() noexcept = default;

private:
  SetT<std::string_view> strtbl_;
  std::deque<std::string> storage_;
};

} // namespace cxxaux
#endif

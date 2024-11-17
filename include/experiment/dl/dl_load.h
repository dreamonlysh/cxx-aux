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

#ifndef ESTD_DL_LOAD_H
#define ESTD_DL_LOAD_H
#include "estd/meta.h"
#include "estd/utility.h"
#include <dlfcn.h>
#include <functional>
#include <optional>

namespace es {

/// \brief DynamicLinkingLibrary is a wrapper of dlfcn
class DynamicLinkingLibrary {
public:
  explicit DynamicLinkingLibrary(void* handler) noexcept : handler(handler) {}
  ~DynamicLinkingLibrary() { reset(); }

  DynamicLinkingLibrary(const DynamicLinkingLibrary&) = delete;
  DynamicLinkingLibrary& operator=(const DynamicLinkingLibrary&) = delete;
  DynamicLinkingLibrary(DynamicLinkingLibrary&& other) noexcept {
    std::swap(handler, other.handler);
  }
  DynamicLinkingLibrary& operator=(DynamicLinkingLibrary&& other) noexcept {
    std::swap(handler, other.handler);
    return *this;
  }

  /// \brief load function from the linking library
  template <typename RetT, typename... Args>
  auto load(const char* fname) const -> RetT (*)(Args...) {
    using FunctionType = RetT (*)(Args...);
    auto func = reinterpret_cast<FunctionType>(dlsym(handler, fname));
    if (dlerror() != nullptr)
      return nullptr;
    return func;
  }

  /// \brief call function in the linking library
  template <typename RetT, typename... Args>
  auto invoke(const char* fname, Args... args) const
      -> std::conditional_t<std::is_void_v<RetT>, void, std::optional<RetT>> {
    auto func = load<RetT, Args...>(fname);
    if (func == nullptr) {
      if constexpr (std::is_void_v<RetT>) {
        return;
      } else {
        return std::nullopt;
      }
    }
    return std::invoke(func, std::forward<Args>(args)...);
  }

  void reset() {
    if (handler == nullptr) {
      dlclose(handler);
      handler = nullptr;
    }
  }

private:
  void* handler{};
};

inline std::optional<DynamicLinkingLibrary> dlLoad(const char* dlPath,
                                                   int mode = RTLD_LAZY) {
  void* handler = dlopen(dlPath, mode);
  if (handler == nullptr)
    return std::nullopt;
  return std::make_optional<DynamicLinkingLibrary>(handler);
}

#ifdef LM_ID_NEWLM
/// \brief strictly load dl as local
/// The situation met is hacking a dl by another dl. For example, to hack
/// libxxx.so, create a new libxxx.so that uses dl load to invoke the old,
/// dlopen is failed with recursive call, but dlmopen with new lm makes it.
inline std::optional<DynamicLinkingLibrary>
dlLoadNewLM(const char* dlPath,
            int mode = RTLD_LAZY | RTLD_LOCAL | RTLD_DEEPBIND) {
  void* handler = dlmopen(LM_ID_NEWLM, dlPath, mode);
  if (handler == nullptr) {
    return std::nullopt;
  }
  return std::make_optional<DynamicLinkingLibrary>(handler);
}
#endif
} // namespace es
#endif // ESTD_DYNAMIC_LIBRARY_H

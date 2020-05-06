#ifndef ESTD_DYNAMIC_LIBRARY_H
#define ESTD_DYNAMIC_LIBRARY_H
#include <dlfcn.h>
#include <optional>
#include "estd/utils.h"
#include "estd/types.h"

namespace estd {

class DynamicLibrary : private disabled_copy {
public:
  explicit DynamicLibrary(void* handler) noexcept
    : handler(handler) {}
  ~DynamicLibrary() {
    dlclose(handler);
  }

  template <typename RetT, typename... Args>
  auto load(CCString name) const -> std::optional<RetT (*)(Args...)> {
    (void)dlerror();
    using FunctionType = RetT (*)(Args...);
    auto func = reinterpret_cast<FunctionType>(dlsym(handler, name));
    if (auto* err = dlerror(); err != nullptr) {
      return std::nullopt;
    }
    return func;
  }

  template <typename RetT, typename... Args>
  auto invoke(CCString name, Args&&... args) const
    -> std::enable_if_t<!std::is_void_v<RetT>, std::optional<RetT>> {
    auto func = load<RetT, Args...>(name);
    if (!func.has_value()) {
      return std::nullopt;
    }
    return std::invoke(func.value(), std::forward<Args>(args)...);
  }

  template <typename RetT, typename... Args>
  auto invoke(CCString name, Args&&... args) const
  -> std::enable_if_t<std::is_void_v<RetT>, bool> {
    auto func = load<RetT, Args...>(name);
    if (func.has_value()) {
      std::invoke(func.value(), std::forward<Args>(args)...);
      return true;
    }
    return false;
  }
private:
  void* handler;
};

std::optional<DynamicLibrary> loadDynamicLibrary(CCString dllPath) {
  void* handler = dlopen(dllPath, RTLD_NOW);
  if (handler == nullptr) return std::nullopt;
  return std::make_optional<DynamicLibrary>(handler);
}

}
#endif //ESTD_DYNAMIC_LIBRARY_H

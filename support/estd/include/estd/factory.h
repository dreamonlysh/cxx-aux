#ifndef ESTD_FACTORY_H
#define ESTD_FACTORY_H
#include <functional>
#include <optional>
#include <map>
#include "utils.h"

namespace estd {

template <typename ConcreteT>
class MemberFunctionFactory {
public:
  template <typename KeyT, typename RetT, typename... Args>
  class FactoryType;

  template <typename FactoryT>
  static constexpr size_t initialize(
      typename std::initializer_list<std::pair<typename FactoryT::key_type,
                                               typename FactoryT::op_type>> init) {
    for (auto p : init)
      FactoryT::holder.emplace(std::move(p));
    return init.size();
  }

  template <typename FactoryT>
  static void initialize_once(
      typename std::initializer_list<std::pair<typename FactoryT::key_type,
                                               typename FactoryT::op_type>> init) {
    static auto dummy = initialize<FactoryT>(std::move(init));
    (void)dummy;
  }

  template <typename FactoryT, typename... Args>
  std::optional<std::enable_if_t<!std::is_void_v<typename FactoryT::ret_type>,
                                 typename FactoryT::ret_type>>
  invoke(typename FactoryT::key_type c, Args&&... args) {
    if (auto it = FactoryT::holder.find(c); it != FactoryT::holder.end()) {
      return std::invoke(it->second, static_cast<ConcreteT*>(this),
                         std::forward<Args>(args)...);
    }
    return std::nullopt;
  }

  template <typename FactoryT, typename... Args>
  std::enable_if_t<std::is_void_v<typename FactoryT::ret_type>>
  invoke(typename FactoryT::key_type c, Args&&... args) {
    if (auto it = FactoryT::holder.find(c); it != FactoryT::holder.end()) {
      std::invoke(it->second, static_cast<ConcreteT*>(this),
                  std::forward<Args>(args)...);
    }
  }
};

template <typename ConcreteT>
template <typename KeyT, typename RetT, typename... Args>
class MemberFunctionFactory<ConcreteT>::FactoryType {
private:
  friend MemberFunctionFactory<ConcreteT>;

  using key_type = KeyT;
  using ret_type = RetT;
  using op_type = RetT (ConcreteT::*)(Args...);

  using CreatorHolder = std::map<key_type, op_type>;
  static CreatorHolder holder;
};

template <typename ConcreteT>
template <typename KeyT, typename RetT, typename... Args>
typename MemberFunctionFactory<ConcreteT>::template FactoryType<KeyT, RetT, Args...>::CreatorHolder
    MemberFunctionFactory<ConcreteT>::FactoryType<KeyT, RetT, Args...>::holder = {};

} // estd

#endif // ESTD_FACTORY_H


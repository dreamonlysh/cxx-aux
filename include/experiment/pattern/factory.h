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

#ifndef EXPERIMENT_PATTERN_FACTORY_H
#define EXPERIMENT_PATTERN_FACTORY_H
#include <estd/meta.h>
#include <functional>
#include <map>
#include <optional>
#include <type_traits>

namespace es {

template <typename ConcreteT>
class MemberFunctionFactory {
public:
  template <typename KeyT, typename RetT, typename... Args>
  class FactoryType;

  template <typename FactoryT>
  static constexpr size_t
  initialize(typename std::initializer_list<
             std::pair<typename FactoryT::key_type, typename FactoryT::op_type>>
                 init) {
    for (auto p : init)
      FactoryT::holder.emplace(std::move(p));
    return init.size();
  }

  template <typename FactoryT>
  static void initialize_once(
      typename std::initializer_list<
          std::pair<typename FactoryT::key_type, typename FactoryT::op_type>>
          init) {
    static auto dummy = initialize<FactoryT>(std::move(init));
    (void)dummy;
  }

  template <typename FactoryT, typename... Args>
  auto invoke(typename FactoryT::key_type c, Args&&... args)
      -> std::conditional_t<std::is_void_v<typename FactoryT::ret_type>, void,
                            std::optional<typename FactoryT::ret_type>> {
    auto it = FactoryT::holder.find(c);
    if (it == FactoryT::holder.end()) {
      if constexpr (std::is_void_v<typename FactoryT::ret_type>) {
        return;
      } else {
        return std::nullopt;
      }
    }

    return std::invoke(it->second, static_cast<ConcreteT*>(this),
                       std::forward<Args>(args)...);
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
typename MemberFunctionFactory<ConcreteT>::template FactoryType<
    KeyT, RetT, Args...>::CreatorHolder
    MemberFunctionFactory<ConcreteT>::FactoryType<KeyT, RetT, Args...>::holder =
        {};

} // namespace es

#endif // ESTD_FACTORY_H

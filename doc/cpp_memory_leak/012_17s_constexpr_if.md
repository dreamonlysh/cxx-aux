> Video Link: [C++ Weekly - Ep 18 C++17's constexpr if](https://www.youtube.com/watch?v=qHgM5UdzPQU&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=18)

## Verification Case

`constexpr if`用于在编译期进行分支选择，不满足条件的分支在编译期被丢弃。从某种程度上来说，`constexpr if`有着`#if...#else`类似的效果。举例来看[dynamic_library](https://gitee.com/dreamonlysh/tony/blob/master/support/es/include/es/dl/dynamic_library.h)中`invoke`函数要同时支持返回值是`void`以及其他的场景：

```c++
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
```

基于`c++`当前并未支持对`void`返回值的函数进行`return`，同时`std::optional`也无`void`特化场景，所以针对两种返回值类别的函数，可以使用SFINAE技巧设计为两个实现。但此处的重复代码之多也是令人心烦，使用`constexpr if`可以优化如下：

```c++
  template <typename RetT, typename... Args>
  auto invoke(CCString name, Args&&... args) const {
    auto func = load<RetT, Args...>(name);
    if constexpr (!std::is_void_v<RetT>) {
      if (!func.has_value()) {
        return std::optional<RetT>();
      }
      return std::make_optional(
        std::invoke(func.value(), std::forward<Args>(args)...));
    } else {
      if (func.has_value()) {
        std::invoke(func.value(), std::forward<Args>(args)...);
        return true;
      }
      return false;
    }
  }
```

在[cppinsights](https://cppinsights.io/)使用`c++17`进行对`invoke<int, int, int>`进行展开：

```c++
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    inline std::optional<int> invoke<int, int, int>(CCString name, int && __args1, int && __args2) const
    {
      std::optional<int (*)(int, int)> func = this->load<int, int, int>(name);
      if constexpr(!std::is_void_v<int>) {
        if(!static_cast<const std::__optional_storage_base<int (*)(int, int), false>&>(func).has_value()) {
          return std::optional<int>();
        }
        
        return std::make_optional(std::invoke(func.value(), std::forward<int>(__args1), std::forward<int>(__args2)));
      }
      
    }
    #endif
```

`else`分支已被丢弃，只剩下常`true`的分支。

当然，以上代码的重复度依然非常惨烈，同时可读性也未必比SFINAE好，同时`std::optional`的`return`写法也比较啰嗦。如此先用`std::conditional`解决`return std::optional`无法使用`std::nullopt`的场景，以及需要显式使用`std::make_optional`的麻烦：

```c++
  template <typename RetT, typename... Args>
  auto invoke(CCString name, Args&&... args) const
  -> std::conditional_t<!std::is_void_v<RetT>, std::optional<RetT>, bool> {
    auto func = load<RetT, Args...>(name);
    if constexpr (!std::is_void_v<RetT>) {
      if (!func.has_value()) {
        return std::nullopt;
      }
      return std::invoke(func.value(), std::forward<Args>(args)...);
    } else {
      if (func.has_value()) {
        std::invoke(func.value(), std::forward<Args>(args)...);
        return true;
      }
      return false;
    }
  }
```

`std::invoke`由于返回值`void`的关系暂无力解决重复，而`if (!func.has_value())`还有机会：

```c++
  template <typename RetT, typename... Args>
  auto invoke(CCString name, Args&&... args) const
  -> std::conditional_t<!std::is_void_v<RetT>, std::optional<RetT>, bool> {
    auto func = load<RetT, Args...>(name);
    if (!func.has_value()) {
      if constexpr (!std::is_void_v<RetT>)
        return std::nullopt;
      else
        return false;
    }

    if constexpr (!std::is_void_v<RetT>) {
      return std::invoke(func.value(), std::forward<Args>(args)...);
    } else {
      std::invoke(func.value(), std::forward<Args>(args)...);
      return true;
    }
  }
```

再根据上述代码分析，可以抽象出两个模型，`conditional`的`type`以及`conditional`的`initial value`。所以进行如下封装：

```c++
/// To replace void with VoidSubstituteT and none void with NoneVoidSubstituteT.
template <typename MayVoidT,
          typename VoidSubstituteT, typename NoneVoidSubstituteT = MayVoidT>
struct substitute_void {
  using type = std::conditional_t<std::is_void_v<MayVoidT>,
                                  VoidSubstituteT, NoneVoidSubstituteT>;
};
template <typename MayVoidT,
          typename VoidSubstituteT, typename NoneVoidSubstituteT = MayVoidT>
using substitute_void_t = typename substitute_void<MayVoidT,
                                                   VoidSubstituteT,
                                                   NoneVoidSubstituteT>::type;

/// Choose the initialization value for substitute_void_t.
template <typename MayVoidT,
          typename VoidSubstituteT, typename NoneVoidSubstituteT>
constexpr auto substitute_void_v(VoidSubstituteT&& t1,
                                 NoneVoidSubstituteT&& t2) {
  if constexpr (std::is_void_v<MayVoidT>) {
    return t1;
  } else {
    return t2;
  }
}
```

`substitute_void_t`与`substitute_void_v`分别对应类型替换和初始值替换：

```c++
  template <typename RetT, typename... Args>
  auto invoke(CCString name, Args&&... args) const
  -> substitute_void_t<RetT, bool, std::optional<RetT>> {
    auto func = load<RetT, Args...>(name);
    if (!func.has_value()) {
      return substitute_void_v<RetT>(false, std::nullopt);
    }

    if constexpr (std::is_void_v<RetT>) {
      std::invoke(func.value(), std::forward<Args>(args)...);
      return true;
    } else {
      return std::invoke(func.value(), std::forward<Args>(args)...);
    }
  }
```

代码可读性以及圈复杂度都达到一个可以接受的标准。

## 结论

直观来看，`constexpr if`可以比较好的替代`std::enable_if`的SFINAE的编译期的重载方式，可以有效的减少重复代码，提高代码的可读性。多数情况下可以使用其来替代SFINAE，并写出更简洁的代码。

这个特性说明`c++`委员会在尝试推动“像写`c++`一样进行元编程”，降低元编程的复杂度。

## 参考资料

[1] conditional. [https://en.cppreference.com/w/cpp/types/conditional](https://en.cppreference.com/w/cpp/types/conditional)

[2] if statement. [https://en.cppreference.com/w/cpp/language/if](https://en.cppreference.com/w/cpp/language/if)
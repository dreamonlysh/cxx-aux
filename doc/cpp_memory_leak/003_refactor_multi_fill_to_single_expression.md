> 基于[002_refactor_char_operation_to_utility](https://gitee.com/dreamonlysh/tony/blob/master/doc/cpp_memory_leak/002_refactor_char_operation_to_utility.md)中的初始化，依然有两点对于应用不友好：
>
> 1. 多次书写单个参数不同的`fill`
>
> 2. 最期望的模式还是`a = b`，而不用额外写初始化函数`genValidChars`
>
> 待优化的代码如下：
>
> ```c++
> constexpr std::array<bool, CHAR_MAX> genValidChars() {
>   std::array<bool, CHAR_MAX> chars= {false};
>   estd::fill(chars, {'_', '$', ';', '/', '|', '.', '?', '@'}, true);
>   estd::fill(chars, estd::meta::char_slice_number, true);
>   estd::fill(chars, estd::meta::char_slice_lower_alphabet, true);
>   estd::fill(chars, estd::meta::char_slice_upper_alphabet, true);
>   return chars;
> }
> 
> constexpr std::array<bool, CHAR_MAX> chars2 = genValidChars();
> ```

## Optimization Case

避免多次书写单个参数不同的`fill`，最直观的想法使用`for`，但无法解决不同类型的问题。处理遍历不同类型自然而然想到的是变参模板：

```c++
namespace estd {
template <typename ContainerT, typename ValueT>
constexpr void fill_n(ContainerT& cont, ValueT&& value) { }

template <typename ContainerT, typename ValueT, typename Arg1, typename... Args>
constexpr void fill_n(ContainerT& cont, ValueT&& value, Arg1&& arg1, Args&&... args) {
  estd::fill(cont, std::forward<Arg1>(arg1), value);
  fill_multi(cont, value, std::forward<Args>(args)...);
}
}

constexpr std::array<bool, CHAR_MAX> genValidChars() {
  std::array<bool, CHAR_MAX> chars= {false};
  estd::fill_n(chars, true,
    std::initializer_list<char>{'_', '$', ';', '/', '|', '.', '?', '@'},
    estd::char_slice_number,
    estd::char_slice_lower_alphabet,
    estd::char_slice_upper_alphabet);
  return chars;
}
```

`fill_n`还有比较神奇的实现：

```c++
template <typename ContainerT, typename ValueT, typename... Args>
constexpr void fill_n(ContainerT& cont, ValueT&& value, Args&&... args) {
  (void)std::initializer_list<int>{
    (estd::meta::fill(cont, std::forward<Args>(args), value), 0)...
  };
}
```

若要实现`a = b`的初始化模式，而不用额外增加初始化函数，最直观的，可以考虑将容器的构建与初始化放入`fill_n`并返回：

```c++
namespace estd {
template <typename ContainerT, typename ValueT, typename... Args>
constexpr ContainerT partial_initializer(ValueT&& value, Args&&... args) {
  ContainerT cont{};
  (void)std::initializer_list<int>{
      (estd::meta::fill(cont, std::forward<Args>(args), value), 0)...
  };
  return cont;
}
}

constexpr auto chars = estd::partial_initializer<std::array<bool, CHAR_MAX>>(
  true,
  std::initializer_list<char>{'_', '$', ';', '/', '|', '.', '?', '@'},
  estd::char_slice_number,
  estd::char_slice_lower_alphabet,
  estd::char_slice_upper_alphabet
);
```

> 完整代码连接：
>
> https://gitee.com/dreamonlysh/tony/tree/master/support/estd/include/estd/meta/fill.h

## 参考资料

[1] Parameter pack。[https://en.cppreference.com/w/cpp/language/parameter_pack](https://en.cppreference.com/w/cpp/language/parameter_pack)
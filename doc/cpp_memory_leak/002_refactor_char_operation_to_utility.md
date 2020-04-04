> 基于[001_refactor_logic_to_static]([https://gitee.com/dreamonlysh/tony/blob/master/doc/Notes4C++Weekly/Ep%201%20ChaiScript_Parser%20Initialization%20Refactor.md](https://gitee.com/dreamonlysh/tony/blob/master/doc/Notes4C++Weekly/Ep 1 ChaiScript_Parser Initialization Refactor.md))中的模块提供更为通用的抽象，凸显主题业务，并提高代码可复用性。
>
> 待优化的代码如下：
>
> ```c++
> constexpr size_t kSpecialCharSize = 8;
> constexpr std::array<bool, CHAR_MAX> genValidChars() {
>   std::array<bool, CHAR_MAX> chars= {false};
>   std::array<char, kSpecialCharSize> specialChars= {
>     '_', '$', ';', '/', '|', '.', '?', '@'
>   };
>   for (size_t i = 0; i < kSpecialCharSize; ++i) {
>     chars[specialChars[i]] = true;
>   }
>   for (char c = '0'; c <= '9'; ++c) {
>     chars[c] = true;
>   }
>   for (char c = 'a'; c <= 'z'; ++c) {
>     chars[c] = true;
>   }
>   for (char c = 'A'; c <= 'Z'; ++c) {
>     chars[c] = true;
>   }
>   return chars;
> }
> constexpr std::array<bool, CHAR_MAX> chars = genValidChars();
> 
> inline bool validChar(char c) {
>   return c < 0 || chars[c];
> }
> ```

## Optimization Case

其中最为明显的`'0' -> '9'`，`'a' -> 'z'`，`'A' -> 'Z'`赋值逻辑完全一致，即有了第一次重构，提取公共的数组范围赋值函数：

```c++
namespace estd {
// Fill the array with `value` on the range positions [first, last).
template <typename T, size_t N>
constexpr void fill(std::array<T, N>& cont, size_t first, size_t last, T&& value) {
  for (; first != last; ++first) {
    cont[first] = value;
  }
}
}

constexpr size_t kSpecialCharSize = 8;
constexpr std::array<bool, CHAR_MAX> genValidChars() {
  std::array<bool, CHAR_MAX> chars= {false};
  std::array<char, kSpecialCharSize> specialChars= {
    '_', '$', ';', '/', '|', '.', '?', '@'
  };
  for (size_t i = 0; i < kSpecialCharSize; ++i) {
    chars[specialChars[i]] = true;
  }
  estd::fill(chars, '0', '9' + 1, true);
  estd::fill(chars, 'a', 'z' + 1, true);
  estd::fill(chars, 'A', 'Z' + 1, true);
  return chars;
}
```

*思考：为什么`fill`函数的范围区间设置为`[,)`，而非`[,]`？*

`接着，'9' + 1`，`'z' + 1`，`'Z' + 1`依然不够优雅，没人喜欢每次都额外写一个`+1`，在根据这三者都具备**range**这样的语义，且在解析器中也常使用，可以适当封装为一个代码范围的类型：

```c++
namespace estd {
template <typename T>
struct Range {
  T first;
  T last;
};

constexpr Range<char> number_char_range{'0', '9' + 1};
constexpr Range<char> lower_alphabet_char_range{'a', 'z' + 1};
constexpr Range<char> upper_alphabet_char_range{'A', 'Z' + 1};
}

constexpr size_t kSpecialCharSize = 8;
constexpr std::array<bool, CHAR_MAX> genValidChars() {
  std::array<bool, CHAR_MAX> chars= {false};
  std::array<char, kSpecialCharSize> specialChars= {
    '_', '$', ';', '/', '|', '.', '?', '@'
  };
  for (size_t i = 0; i < kSpecialCharSize; ++i) {
    chars[specialChars[i]] = true;
  }
  estd::fill(chars, estd::number_char_range.first, estd::number_char_range.last, true);
  estd::fill(chars, estd::lower_alphabet_char_range.first, estd::lower_alphabet_char_range.last, true);
  estd::fill(chars, estd::upper_alphabet_char_range.first, estd::upper_alphabet_char_range.last, true);
  return chars;
}
```

不知道是否大家都对`c++`算法库中，需要调用容器`begin()`、`end()`这种写法深感啰嗦。至少上述代码`fill`对于调用者并不友好（他奶奶的裹脚布）。那么提供一个基于范围`fill`势在必行：

```c++
namespace estd {
template <typename T, size_t N, typename RangeT>
constexpr void fill(std::array<T, N>& cont, RangeT&& range, T&& value) {
  estd::fill(cont, range.first, range.last, std::forward<T>(value));
}
}

constexpr size_t kSpecialCharSize = 8;
constexpr std::array<bool, CHAR_MAX> genValidChars() {
  std::array<bool, CHAR_MAX> chars= {false};
  std::array<char, kSpecialCharSize> specialChars = {
    '_', '$', ';', '/', '|', '.', '?', '@'
  };
  for (size_t i = 0; i < kSpecialCharSize; ++i) {
    chars[specialChars[i]] = true;
  }
  estd::fill(chars, estd::number_char_range, true);
  estd::fill(chars, estd::lower_alphabet_char_range, true);
  estd::fill(chars, estd::upper_alphabet_char_range, true);
  return chars;
}
```

**像我们这样没有博士学位的凡人**至少还是知道数学中**离散**和**连续**模型的，`specialChars`和`number_char_range`倒是可以看做这两类，也就是说下一步可尝试使`fill`支持`specialChars`。**连续**套用了概念`range`，**离散**从上述`{'_', '$', ';', '/', '|', '.', '?', '@'}`中最容易推导出的是**初始化列表**：

```c++
namespace estd {
template <typename T, size_t N, typename PositionT>
constexpr void fill(std::array<T, N>& cont,
                    std::initializer_list<PositionT> pos, T&& value) {
  for (auto it = pos.begin(); it != pos.end(); ++it) {
    cont[*it] = value;
  }
}
}

constexpr std::array<bool, CHAR_MAX> genValidChars() {
  std::array<bool, CHAR_MAX> chars= {false};
  estd::fill(chars, {'_', '$', ';', '/', '|', '.', '?', '@'}, true);
  estd::fill(chars, estd::number_char_range, true);
  estd::fill(chars, estd::lower_alphabet_char_range, true);
  estd::fill(chars, estd::upper_alphabet_char_range, true);
  return chars;
}
```

至此我们已经提供了针对编译期数组计算的`fill`函数，同时定义的`Range`类。但回顾来看，`Range`的设计引入了`first`/`last`的隐式约定，应尽量避免引入新的概念，减少调用复杂度，此处可以考虑`begin`/`end`语义；同时，`number_char_range`、`lower_alphabet_char_range`、`upper_alphabet_char_range`具有概念的连续性，可以考虑更有意义的命名，如**切片**；再者，参考`c++`中类似的设计，如`std::numeric_limits`，其设计导向倾向于提供接口而非数据成员的直接访问。

```c++
namespace estd {
template <typename T, typename Size = size_t>
class range_slice {
public:
  using value_type = T;
  using size_type = Size;

  constexpr range_slice(T first, T last)
    : first(first), last(last) {}
  ~range_slice() = default;

  constexpr value_type begin() const { return first; }
  constexpr value_type end() const { return last; }
  constexpr size_type size() const { return last - first; }

private:
  T first;
  T last;
};

constexpr auto char_slice_number = range_slice<char>('0', '9' + 1);
constexpr auto char_slice_lower_alphabet = range_slice<char>('a', 'z' + 1);
constexpr auto char_slice_upper_alphabet = range_slice<char>('A', 'Z' + 1);
}
```

> 完整代码连接：
>
> https://gitee.com/dreamonlysh/tony/tree/master/support/estd/include/estd/meta/range.h
>
> https://gitee.com/dreamonlysh/tony/tree/master/support/estd/include/estd/meta/fill.h

## 参考资料

[1] constexpr 说明符。[https://zh.cppreference.com/w/cpp/language/constexpr](https://zh.cppreference.com/w/cpp/language/constexpr)

[2] 算法库。[https://zh.cppreference.com/w/cpp/algorithm](https://zh.cppreference.com/w/cpp/algorithm)

[3] std::array。[https://zh.cppreference.com/w/cpp/container/array](https://zh.cppreference.com/w/cpp/container/array)


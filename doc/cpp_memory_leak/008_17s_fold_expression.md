本文用于初步探索`C++17`引入的`fold expression`特性来简化`parameter pack`的使用。

## Verification Case

在"003_refactor_multi_fill_to_single_expression"中，我们将

```c++
template <typename ContainerT, typename ValueT>
constexpr void fill_n(ContainerT& cont, ValueT&& value) { }

template <typename ContainerT, typename ValueT, typename Arg1, typename... Args>
constexpr void fill_n(ContainerT& cont, ValueT&& value, Arg1&& arg1, Args&&... args) {
  estd::fill(cont, std::forward<Arg1>(arg1), value);
  fill_n(cont, value, std::forward<Args>(args)...);
}
```

优化为

```c++
template <typename ContainerT, typename ValueT, typename... Args>
constexpr void fill_n(ContainerT& cont, ValueT&& value, Args&&... args) {
  (void)std::initializer_list<int>{
    (estd::fill(cont, std::forward<Args>(args), value), 0)...
  };
}
```

彼时相当诧异，`c++`竟然没有提供一种文法来避免这种啰嗦的语句，巧的是`mlir`[PatternMatch.h:413 FIXME](https://github.com/llvm/llvm-project/blob/master/mlir/include/mlir/IR/PatternMatch.h)的参数包展开中提及在`c++17`使用`fold expression`来优化代码写法。即在可以优化为：

```c++
template <typename ContainerT, typename ValueT, typename... Args>
constexpr void fill_n(ContainerT& cont, ValueT&& value, Args&&... args) {
  (estd::fill(cont, std::forward<Args>(args), value) , ...);
}
```

或

```c++
template <typename ContainerT, typename ValueT, typename... Args>
constexpr void fill_n(ContainerT& cont, ValueT&& value, Args&&... args) {
  (... , estd::fill(cont, std::forward<Args>(args), value));
}
```

可以看到此处`,`前留有空格，这个在`cppreference`<sup>[1]</sup>中将`,`等同于二元操作符，所以此处遵循二元操作符前后留有空格的风格。

## `cppreference`关键点

### 展开过程

> The instantiation of a *fold expression* expands the expression `e` as follows:
>
> 1) Unary right fold *(E op ...)* becomes *(E<sub>1</sub> op (... op (E<sub>N-1</sub> op E<sub>N</sub>)))*
>
> 2) Unary left fold *(... op E)* becomes *(((E<sub>1</sub> op E<sub>2</sub>) op ...) op E<sub>N</sub>)*
>
> 3) Binary right fold *(E op ... op I)* becomes *(E<sub>1</sub> op (... op (E<sub>N−1</sub> op (E<sub>N</sub> op I))))*
>
> 4) Binary left fold *(I op ... op E)* becomes *((((I op E<sub>1</sub>) op E<sub>2</sub>) op ...) op E<sub>N</sub>)*
>
> (where *N* is the number of elements in the pack expansion)

此处明确的指明了展开顺序，以及通过`()`明确的运算优先级。这点是`std::initializer_list`所不具备([C++ Weekly - Ep 10 Variadic Expansion Wrap-Up](https://www.youtube.com/watch?v=CU3VYN6xGzM&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=10)验证了`gcc`和`clang`，`gcc`逆序展开)。

### 场景演示

其中`op`当前共支持以下32种场景：`+` `-` `*` `/` `%` `^` `&` `|` `=` `<` `>` `<<` `>>` `+=` `-=` `*=` `/=` `%=` `^=` `&=` `|=` `<<=` `>>=` `==` `!=` `<=` `>=` `&&` `||` `,` `.*` `->*`

1. 算数/位运算：`+` `-` `*` `/` `%` `^` `&` `|` `<<` `>>`

```c++
template <typename... Args>
int sum(Args&&... args) {
  return (args + ... + 0);
}
#ifdef INSIGHTS_USE_TEMPLATE
template<>
int sum<int, int, int, int>(int && __args0, int && __args1, int && __args2, int && __args3)
{
  return __args0 + (__args1 + (__args2 + (__args3 + 0)));
}
#endif
// ASSERT_EQ(sum(1, 2, 3, 4), 10);
```

```c++
template <typename... Args>
int sum(Args&&... args) {
  return (0 + ... + args);
}
#ifdef INSIGHTS_USE_TEMPLATE
template<>
int sum<int, int, int, int>(int && __args0, int && __args1, int && __args2, int && __args3)
{
  return (((0 + __args0) + __args1) + __args2) + __args3;
}
#endif
// ASSERT_EQ(sum(1, 2, 3, 4), 10);
```

2. 算数/位赋值运算：`=` `+=` `-=` `*=` `/=` `%=` `^=` `&=` `|=` `<<=` `>>=`

```c++
template <typename... Args>
int assign(Args&&... args) {
  int result = -1;
  (result = ... = args);
  return result;
}
#ifdef INSIGHTS_USE_TEMPLATE
template<>
int assign<int, int, int, int>(int && __args0, int && __args1, int && __args2, int && __args3)
{
  int result = -1;
  (((result = __args0) = __args1) = __args2) = __args3;
  return result;
}
#endif
// ASSERT_EQ(assign(1, 2, 3, 4), 4);
```

注意，此处没有`(args = ... = result)`，因为向字面值`1 = 2`赋值是不成立的。

3. 比较/逻辑运算<sup>[1]</sup>：`<` `>` `==` `!=` `<=` `>=`  `&&` `||`

```c++
template<typename... Args>
bool all(Args... args) {
  return (... && args);
}
 
bool b = all(true, true, true, false);
// within all(), the unary left fold expands as
//  return ((true && true) && true) && false;
// b is false
```

4. 流操作：`<<` `>>`

```c++
template <typename... Args>
template <typename... Args>
void print(Args&&... args) {
  (std::cout << ... << std::forward<Args>(args));
}

/* First instantiated from: insights.cpp:12 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
void print<int, int, int, int>(int && __args0, int && __args1,
                               int && __args2, int && __args3)
{
  std::cout.operator<<(std::forward<int>(__args0))
           .operator<<(std::forward<int>(__args1))
           .operator<<(std::forward<int>(__args2))
           .operator<<(std::forward<int>(__args3));
}
#endif
// print(1, 2, 3, 4); // 1234
```

5. 表达式展开：`,`

```c++
template <typename... Args>
void print(Args&&... args) {
  ((std::cout << std::forward<Args>(args)) , ...);
}
#ifdef INSIGHTS_USE_TEMPLATE
template<>
void print<int, int, int, int>(int && __args0, int && __args1, int && __args2, int && __args3)
{
  (std::cout.operator<<(std::forward<int>(__args0))) ,
  (
    (std::cout.operator<<(std::forward<int>(__args1))) ,
    (
      (std::cout.operator<<(std::forward<int>(__args2))) ,
      (std::cout.operator<<(std::forward<int>(__args3)))
    )
  );
}
#endif
```

6. 成员递归展开<sup>[2]</sup>：`.*` `->*`

```c++
struct Node {
int value;
  Node* left ;
  Node* right ;
};

auto left = &Node::left;
auto right = &Node::right;

template <typename T, typename... TD >
Node* traverse (T start , TD... paths) {
  return (start ->* ... ->* paths);
}

#ifdef INSIGHTS_USE_TEMPLATE
template<>
Node * traverse<Node *, Node *Node::*, Node *Node::*, Node *Node::*>(Node * start, Node *Node::* __paths1, Node *Node::* __paths2, Node *Node::* __paths3)
{
  return ((start ->* __paths1) ->* __paths2) ->* __paths3;
}
#endif
// Node * root = nullptr;
// traverse(root, left, right, left); // ((root ->* left) ->* right) ->* left
```

*实在想象不出来哪种场景会如此设计模块，这得多想不开。*

## 参考资料

[1] fold expression. [https://en.cppreference.com/w/cpp/language/fold](https://en.cppreference.com/w/cpp/language/fold)

[2] Fold Expressions in C++17. CHRISTOPH HONAL, Technical University of Munich, Germany. [https://wiki.tum.de/download/attachments/93291100/Honal%20report%20-%20Fold%20Expressions%20in%20C%2B%2B17.pdf?version=1&modificationDate=1532345182147&api=v2](https://wiki.tum.de/download/attachments/93291100/Honal%20report%20-%20Fold%20Expressions%20in%20C%2B%2B17.pdf?version=1&modificationDate=1532345182147&api=v2)
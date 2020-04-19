> Video Link: [C++ Weekly - Ep 15 Using `std::bind`](https://www.youtube.com/watch?v=JtUZmkvroKg&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=15), [C++ Weekly - Ep 16 Avoiding `std::bind`](https://www.youtube.com/watch?v=ZlHi8txU4aQ&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=16)

## Verification Case

示例如下，有`add`函数：

```c++
int add(int a, int b) {
  return a + b;
}
```

现要通过其支持步进`1`(`add(i, 1)`)，使用`std::bind`：

```c++
auto increase = std::bind(add, std::placeholders::_1, 1);
ASSERT_EQ(increase(1), 2);
```

`std::placeholders::_1`代表传入`increase`的第一个参数，此时看来还是挺不错的。若传递的并非`1`，而是一个可变对象的引用，那使用`std::bind`代码会是这样：

```c++
int step = 1;
auto increase = std::bind(add, std::placeholders::_1, std::ref(step));
ASSERT_EQ(increase(1), 2);
step = 2;
ASSERT_EQ(increase(1), 3);
```

`std::ref`代表`add`的第二个参数为引用绑定，如此后续更改才会生效。若`add`为模板函数：

```c++
template <typename T>
T add(T a, T b) {
  return a + b;
}

int step = 1;
auto increase = std::bind(add<int>, std::placeholders::_1, std::ref(step));
ASSERT_EQ(increase(1), 2);
step = 2;
ASSERT_EQ(increase(1), 3);
```

则必须指定`add<int>`。若`add`参数为右值：

```c++
template <typename T, typename U>
T add(T&& a, U&& b) {
  return a + b;
}
```

则报错如下：

> xxx.cpp:xx : error: no matching function for call to object of type 'std::__1::__bind<int (&)(int &&, int &&), const std::__1::placeholders::__ph<1> &, std::__1::reference_wrapper<int> >'
>   ASSERT_EQ(increase(1), 2);

若使用`lambda`表达式，则以上三段`std::bind`代码可以改造为：

1. 改造拷贝绑定场景

```c++
auto increase = [](int a) {
  return add(a, 1);
};
ASSERT_EQ(increase(1), 2);
```

2. 改造引用绑定场景：

```c++
int step = 1;
auto increase = [&step](int a) {
  return add(a, step);
};
ASSERT_EQ(increase(1), 2);
step = 2;
ASSERT_EQ(increase(1), 3);
```

3. 改造万能引用场景：

```c++
int step = 1;
auto increase = [&step](auto&& a) {
  return add(std::forward<decltype(a)>(a), step);
};
ASSERT_EQ(increase(1), 2);
step = 2;
ASSERT_EQ(increase(1), 3);
```

**此处从简单到复杂列举的3个示例，综合来看使用`lambda`更为接近通常的代码书写，而`std::bind`需要各种额外的支撑结构，表达相对拗口。**

将一下代码进行展开：

```c++
int add(int a, int b) {
  return a + b;
}

int addBind() {
  int step = 1;
  auto increase = std::bind(add, std::placeholders::_1, std::ref(step));
  return increase(1);
}

int addLambda() {
  int step = 1;
  auto increase = [&step](int a) {
    return add(a, step);
  };
  return increase(1);
}
```

可以查看到`std::bind`的`increase`类型为：

```c++
std::_Bind<int (*(std::_Placeholder<1>, std::reference_wrapper<int>))(int, int)>
```

其中`std::_Bind`包含两成员：一个函数指针以及对应参数的`tuple`。

`lambda`的`increase`类型为：

```c++
class __lambda_19_19
{
  public: 
  inline /*constexpr */ int operator()(int a) const
  {
    return add(a, step);
  }
  
  private: 
  int & step;
  
  public:
  __lambda_19_19(int & _step)
  : step{_step}
  {}
};
```

仅仅包含绑定数据作为的成员，比`std::_Bind`简单非常之多。同时在此我们也能大概推测，编译器更有机会对`lambda`做`inline`等优化，而`std::bind`由于包含函数指针的成员(指针一般认为是动态绑定)，编译器能做的优化会少很多。将视频中的代码：

```c++
void print(int i, const std::string& str) __attribute__((noinline));
void print(int i, const std::string& str) {
  std::cout << i << ' ' << str << '\n';
}

void bindPrint() {
  auto p = std::bind(print, std::placeholders::_2, std::placeholders::_1);
  for (int i = 0; i < 1000; ++i) {
    p("std::bind", i);
  }
}

void lambdaPrint() {
  auto p = [](auto&& str, auto&& index) {
    print(index, str);
  };
  for (int i = 0; i < 1000; ++i) {
    p("std::bind", i);
  }
}
```

`gcc9.3 -std=c++17 -O2`生成汇编([godbolt](https://godbolt.org/))完全一致，此处不做展示。而`clang10`生成的汇编，`std::bind`未能解析出函数指针为静态，所以生成大量调用代码，如下：

```assembly
bindPrint():                          # @bindPrint()
        push    r14
        push    rbx
        sub     rsp, 24
        mov     qword ptr [rsp + 8], offset print(int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)
        mov     dword ptr [rsp + 4], 0
        lea     r14, [rsp + 8]
        lea     rbx, [rsp + 4]
.LBB1_1:                                # =>This Inner Loop Header: Depth=1
        mov     edx, offset .L.str
        mov     rdi, r14
        mov     rsi, rbx
        call    void std::__invoke_impl<void, void (*&)(int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&), int&, char const (&) [10]>(std::__invoke_other, void (*&)(int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&), int&, char const (&) [10])
        mov     eax, dword ptr [rsp + 4]
        lea     ecx, [rax + 1]
        mov     dword ptr [rsp + 4], ecx
        cmp     eax, 999
        jl      .LBB1_1
        add     rsp, 24
        pop     rbx
        pop     r14
        ret

void std::__invoke_impl<void, void (*&)(int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&), int&, char const (&) [10]>(std::__invoke_other, void (*&)(int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&), int&, char const (&) [10]): # @void std::__invoke_impl<void, void (*&)(int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&), int&, char const (&) [10]>(std::__invoke_other, void (*&)(int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&), int&, char const (&) [10])
        push    rbp
        push    r15
        push    r14
        push    r12
        push    rbx
        sub     rsp, 48
        mov     rbx, rdx
        mov     r12, qword ptr [rdi]
        mov     r14d, dword ptr [rsi]
        lea     r15, [rsp + 32]
        mov     qword ptr [rsp + 16], r15
        mov     rdi, rdx
        call    strlen
        mov     rbp, rax
        mov     qword ptr [rsp + 8], rax
        mov     rax, r15
        cmp     rbp, 16
        jb      .LBB4_3
        lea     rdi, [rsp + 16]
        lea     rsi, [rsp + 8]
        xor     edx, edx
        call    std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_create(unsigned long&, unsigned long)
        mov     qword ptr [rsp + 16], rax
        mov     rcx, qword ptr [rsp + 8]
        mov     qword ptr [rsp + 32], rcx
.LBB4_3:
        test    rbp, rbp
        je      .LBB4_7
        cmp     rbp, 1
        jne     .LBB4_6
        mov     cl, byte ptr [rbx]
        mov     byte ptr [rax], cl
        jmp     .LBB4_7
.LBB4_6:
        mov     rdi, rax
        mov     rsi, rbx
        mov     rdx, rbp
        call    memcpy
.LBB4_7:
        mov     rax, qword ptr [rsp + 8]
        mov     qword ptr [rsp + 24], rax
        mov     rcx, qword ptr [rsp + 16]
        mov     byte ptr [rcx + rax], 0
        lea     rsi, [rsp + 16]
        mov     edi, r14d
        call    r12
        mov     rdi, qword ptr [rsp + 16]
        cmp     rdi, r15
        je      .LBB4_10
        call    operator delete(void*)
.LBB4_10:
        add     rsp, 48
        pop     rbx
        pop     r12
        pop     r14
        pop     r15
        pop     rbp
        ret
        mov     rdi, rax
        call    _Unwind_Resume
        mov     rbx, rax
        mov     rdi, qword ptr [rsp + 16]
        cmp     rdi, r15
        je      .LBB4_14
        call    operator delete(void*)
.LBB4_14:
        mov     rdi, rbx
        call    _Unwind_Resume
```

```assembly
lambdaPrint():                       # @lambdaPrint()
        push    r15
        push    r14
        push    r12
        push    rbx
        sub     rsp, 40
        xor     ebx, ebx
        lea     r15, [rsp + 24]
        movabs  r12, 7955998218953978995
        lea     r14, [rsp + 8]
        jmp     .LBB2_1
.LBB2_4:                                #   in Loop: Header=BB2_1 Depth=1
        add     ebx, 1
        cmp     ebx, 1000
        je      .LBB2_5
.LBB2_1:                                # =>This Inner Loop Header: Depth=1
        mov     qword ptr [rsp + 8], r15
        mov     qword ptr [r15], r12
        mov     byte ptr [r15 + 8], 100
        mov     qword ptr [rsp + 16], 9
        mov     byte ptr [rsp + 33], 0
        mov     edi, ebx
        mov     rsi, r14
        call    print(int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)
        mov     rdi, qword ptr [rsp + 8]
        cmp     rdi, r15
        je      .LBB2_4
        call    operator delete(void*)
        jmp     .LBB2_4
.LBB2_5:
        add     rsp, 40
        pop     rbx
        pop     r12
        pop     r14
        pop     r15
        ret
        mov     rbx, rax
        mov     rdi, qword ptr [rsp + 8]
        cmp     rdi, r15
        je      .LBB2_8
        call    operator delete(void*)
.LBB2_8:
        mov     rdi, rbx
        call    _Unwind_Resume
```

**从上述`gcc`和`clang`的对比，至少可以认为`lambda`有不弱于`std::bind`的优化性能。**

## 结论

停止使用`std::bind`，使用`lambda`。

## 参考资料

[1] std::bind. [https://zh.cppreference.com/w/cpp/utility/functional/bind](https://zh.cppreference.com/w/cpp/utility/functional/bind)
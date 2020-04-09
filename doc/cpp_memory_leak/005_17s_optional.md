本文用于初步探索`C++17`引入的`std::optional`的使用与性能。

## Verification Case

### 知识补充

如下代码，或多或少还是会见到一些：

```c++
// xxx.h
enum Kind {
  kFirstKind,
  kSecondKind
};

// zzz.cpp
int calcKind(int input) {
  if (input > 0)
    return kFirstKind;
  if (input < 0)
    return kSecondKind;
  return -1;
}

int main() {
  int result = calcKind(1);
  if (result != -1)
    return result;
  return 0;
}
```

这里存在好些问题：

1. 表达上，`kFirstKind`/`kSecondKind`与`-1`归属于不同类型(虽然`decay`类型相同)
2. `Kind`属于接口，`calcKind`属于实现，接口定义不会反依赖实现，而此处`-1`占用了`Kind`的一个可选值(污染)，这在逻辑上是有风险的
3. `-1`有可能在多处使用，而成为一个难以感知的，默认非法值定义，属于隐式接口，并造成散弹式的局面
4. 当前程序定义为`-1`，同样在另外的程序可能定义成`INT32_MAX`

那么使用`std::optional`可以解决此问题，改造后的代码如下：

```c++
// zzz.cpp
std::optional<int> calcKind(int input) {
  if (input > 0)
    return std::make_optional(kFirstKind);
  if (input < 0)
    return std::make_optional(kSecondKind);
  return std::nullopt;
}

int main() {
  auto result = calcKind(1);
  if (result.has_value())
    return result.value();
  return 0;
}
```

以上4个问题几乎都解决掉了，接下来再看下性能影响。

### 函数汇编对比

使用`std::optional`的主要场景是存在分支条件的返回值，构建如下：

```c++
bool cond = true;
int g_int_value = 100;

int g_result = 0;
auto getOrigin() {
  if (cond)
    return g_int_value;
  return -1;
}

std::optional<int> getOpt() {
  if (cond)
    return std::make_optional<int>(g_int_value);
  return std::nullopt;
}

void testGetOrigin() {
  auto v = getOrigin();
  if (v > 0)
    g_result = v;
}

void testGetOpt() {
  auto v = getOpt();
  if (v.has_value())
    g_result = v.value();
}
```

`gcc9.3 -std=c++17 -O2`生成汇编([godbolt](https://godbolt.org/))如下：

```assembly
getOrigin():
        cmp     BYTE PTR cond[rip], 0
        mov     eax, -1
        cmovne  eax, DWORD PTR g_int_value[rip]
        ret
getOpt():
        cmp     BYTE PTR cond[rip], 0
        je      .L6
        mov     eax, DWORD PTR g_int_value[rip]
        mov     BYTE PTR [rsp-4], 1
        mov     DWORD PTR [rsp-8], eax
        mov     rax, QWORD PTR [rsp-8]
        ret
.L6:
        mov     BYTE PTR [rsp-4], 0
        mov     rax, QWORD PTR [rsp-8]
        ret
testGetOrigin():
        cmp     BYTE PTR cond[rip], 0
        je      .L8
        mov     eax, DWORD PTR g_int_value[rip]
        test    eax, eax
        jle     .L8
        mov     DWORD PTR g_result[rip], eax
.L8:
        ret
testGetOpt():
        cmp     BYTE PTR cond[rip], 0
        je      .L10
        mov     eax, DWORD PTR g_int_value[rip]
        mov     DWORD PTR g_result[rip], eax
.L10:
        ret
```

可以看到`getOpt()`函数比`getOrigin`要多不少指令，但其在测试函数被内联后，`testGetOpt()`竟然获得了更优的优化。

再试试`clang10`，生成汇编如下：

```assembly
getOrigin():                          # @getOrigin()
        xor     eax, eax
        cmp     byte ptr [rip + cond], 1
        sbb     eax, eax
        or      eax, dword ptr [rip + g_int_value]
        ret
getOpt():                             # @getOpt()
        movzx   eax, byte ptr [rip + cond]
        mov     ecx, dword ptr [rip + g_int_value]
        shl     rax, 32
        or      rax, rcx
        ret
testGetOrigin():                     # @testGetOrigin()
        xor     eax, eax
        cmp     byte ptr [rip + cond], 1
        sbb     eax, eax
        or      eax, dword ptr [rip + g_int_value]
        test    eax, eax
        jle     .LBB2_2
        mov     dword ptr [rip + g_result], eax
.LBB2_2:
        ret
testGetOpt():                        # @testGetOpt()
        cmp     byte ptr [rip + cond], 0
        je      .LBB3_2
        mov     eax, dword ptr [rip + g_int_value]
        mov     dword ptr [rip + g_result], eax
.LBB3_2:
        ret
```

可以看到`getOpt()`函数和`getOrigin`指令数相同，`testGetOpt()`同样获得了比较优的优化。从当前分析来看，`std::optional`来解决这类问题可行。

当存在多路径返回时，很多情况下是无法进行内联的，所以测试下禁用内联的情况：

```c++
auto getOrigin() __attribute__((noinline));
std::optional<int> getOpt() __attribute__((noinline));
```

`gcc9.3 -std=c++17 -O2`生成汇编([godbolt](https://godbolt.org/))如下：

```assembly
testGetOrigin():
        call    getOrigin()
        test    eax, eax
        jle     .L8
        mov     DWORD PTR g_result[rip], eax
.L8:
        ret
testGetOpt():
        sub     rsp, 16
        call    getOpt()
        mov     rdx, rax
        mov     QWORD PTR [rsp+8], rax
        shr     rdx, 32
        test    dl, dl
        je      .L10
        mov     eax, DWORD PTR [rsp+8]
        mov     DWORD PTR g_result[rip], eax
.L10:
        add     rsp, 16
        ret
```

`gcc`下情况比较坏，再试试`clang10`，生成汇编如下：

```assembly
testGetOrigin():                     # @testGetOrigin()
        push    rax
        call    getOrigin()
        test    eax, eax
        jle     .LBB2_2
        mov     dword ptr [rip + g_result], eax
.LBB2_2:
        pop     rax
        ret
testGetOpt():                        # @testGetOpt()
        push    rax
        call    getOpt()
        movabs  rcx, 1095216660480
        test    rax, rcx
        je      .LBB3_2
        mov     dword ptr [rip + g_result], eax
.LBB3_2:
        pop     rax
        ret
```

`clang`的优化效果非常好，也比较稳定，相对而言`gcc`对于对象本身的优化似乎有所缺失。虽然`gcc`有所缺失，但咱不能因此而否定`std::optional`，此处依然可行。

再试试`std::string`对比下(此处只附结论)：

`gcc9.3 -std=c++17 -O2`生成汇编([godbolt](https://godbolt.org/))如下：

`getOrigin`: 20, `testGetOrigin`: 20 + [clone .cold]7

`getOpt`: 18, `testGetOpt`:25 + [clone .cold]10

`clang10 -std=c++17 -O2`命令下使用`std::optional`与不适用情况下，指令数几乎相等。(从上述数据来看，`clang`的编译优化相对比较稳定，`gcc`的优化波动比较大)。

题外话，再对比下`std::optional`内存占用：

```c++
int32_t i = 0;
std::optional<int32_t> io = std::make_optional<int32_t>(1);

int64_t l = 0;
std::optional<int64_t> lo = std::make_optional<int64_t>(1);

std::string s = "0";
std::optional<std::string> so = std::make_optional<std::string>("1");

struct A {
  int64_t a;
  int32_t b;
};

A a;
std::optional<A> ao;
```

`gcc9.3 -std=c++17 -O2`生成汇编([godbolt](https://godbolt.org/))如下：

```assembly
ao:
        .quad   0
        .long   0
        .zero   4
        .byte   1
        .zero   7
a:
        .zero   16
so[abi:cxx11]:
        .zero   40
s[abi:cxx11]:
        .zero   32
lo:
        .quad   1
        .byte   1
        .zero   7
l:
        .zero   8
io:
        .long   1
        .byte   1
        .zero   3
i:
        .zero   4
```

比较明显，`std::optional`并未在控制内存上做出什么努力，依然额外占用了`bit->byte->align(T)`的空间。这点确实没有惊喜，本期望上例中`std::optional`中用作判断的数据可以优化到`struct A`中补齐的4bytes。

## 结论

在编译器优化的波动范围内，

1. 作为返回值时，`std::optional`与传统方式拥有几乎相同的性能，所以推荐使用
2. 作为对象成员或全局对象时，由于`std::optional`没有内存优化，所以不推荐使用，即不推荐存储`std::optional`类型的数据。
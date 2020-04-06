> Video Link: [C++ Weekly - Ep 2 Cost of Using Statics](https://www.youtube.com/watch?v=B3WWsKFePiM&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=2)

此视频的目的是探索`static`局部变量的开销。

## Verification Case

### 知识补充

`c++ 11`前单件的实现需要线程安全的双重校验，其实现可能如下：

```c++
class Singleton {
public:
  static Instance& ins() {
    if (instance == NULL) {
      lock_guard();
      if (instance == NULL) {
        instance = new Instance();
      }
    }
    return *instance;
  }
private:
  static Instance* instance;
};
Instance* Singleton::instance = NULL;
```

`c++ 11`开始提供静态局部变量的线程安全初始化<sup>[2]</sup>，则单件可实现为：

```c++
class Singleton {
public:
  static Instance& ins() {
    static Instance* instance;
    return instance;
  }
};
```

因为好用，所以更容易被滥用。

### 函数汇编对比

```c++
std::string& getName() {
  static std::string s(" ");
  return s;
}

std::string& name = getName();
std::string& getNameCached() {
  return name;
}
```

`gcc9.3 -std=c++17 -O2`生成汇编([godbolt](https://godbolt.org/))如下：

```assembly
getName[abi:cxx11]():
        movzx   eax, BYTE PTR guard variable for getName[abi:cxx11]()::s[rip]
        test    al, al
        je      .L13
        mov     eax, OFFSET FLAT:getName[abi:cxx11]()::s
        ret
.L13:
...
getNameCached[abi:cxx11]():
        mov     rax, QWORD PTR name[abi:cxx11][rip]
        ret
```

其中`getName`函数每次调用时均需要做一次初始化校验，比`getNameCached`多了**3条指令**。

继续验证调用场景：

```c++
size_t testGetName() {
  size_t count = 0;
  for (size_t i = 0; i < 1000000; ++i) {
    count += getName().size();
  }
  return count;
}

size_t testGetNameCached() {
  size_t count = 0;
  for (size_t i = 0; i < 1000000; ++i) {
    count += getNameCached().size();
  }
  return count;
}
```

`gcc9.3 -std=c++17 -O2`生成汇编([godbolt](https://godbolt.org/))如下：

```assembly
testGetName():
        push    r12
        xor     r12d, r12d
        push    rbx
        mov     ebx, 1000000
        sub     rsp, 8
.L17:
        call    getName[abi:cxx11]()
        add     r12, QWORD PTR [rax+8]
        sub     rbx, 1
        jne     .L17
        add     rsp, 8
        mov     rax, r12
        pop     rbx
        pop     r12
        ret
testGetNameCached():
        mov     rax, QWORD PTR name[abi:cxx11][rip]
        imul    rax, QWORD PTR [rax+8], 1000000
        ret
```

结果比较戏剧化，`getNameCached`行为被硬件指令支持并`inlne`了。

当然此结果不具备通配性，试着修改函数，禁用`inline`(上例中`getName`未能`inline`，拉齐规格)，同时将循环此处由外部传入(移除`imul`的极致优化)：

```c++
std::string& getName() __attribute__((noinline));
std::string& getNameCached() __attribute__((noinline));

std::string& getName() {
  static std::string s(" ");
  return s;
}

std::string& name = getName();
std::string& getNameCached() {
  return name;
}

size_t testGetName(size_t n) {
  size_t count = 0;
  for (size_t i = 0; i < n; ++i) {
    count += getName().size();
  }
  return count;
}

size_t testGetNameCached(size_t n) {
  size_t count = 0;
  for (size_t i = 0; i < n; ++i) {
    count += getNameCached().size();
  }
  return count;
}
```

`gcc9.3 -std=c++17 -O2`生成汇编([godbolt](https://godbolt.org/))如下：

```assembly
testGetName(unsigned long):                       # @testGetName(unsigned long)
        push    r14
        push    rbx
        push    rax
        test    rdi, rdi
        je      .LBB2_1
        mov     rbx, rdi
        xor     r14d, r14d
.LBB2_4:                                # =>This Inner Loop Header: Depth=1
        call    getName[abi:cxx11]()
        add     r14, qword ptr [rip + getName[abi:cxx11]()::s[abi:cxx11]+8]
        add     rbx, -1
        jne     .LBB2_4
        jmp     .LBB2_2
.LBB2_1:
        xor     r14d, r14d
.LBB2_2:
        mov     rax, r14
        add     rsp, 8
        pop     rbx
        pop     r14
        ret
testGetNameCached(unsigned long):                 # @testGetNameCached(unsigned long)
        push    rbx
        test    rdi, rdi
        je      .LBB3_1
        mov     rbx, rdi
        call    getNameCached[abi:cxx11]()
        imul    rbx, qword ptr [rax + 8]
        jmp     .LBB3_3
.LBB3_1:
        xor     ebx, ebx
.LBB3_3:
        mov     rax, rbx
        pop     rbx
        ret
```

乍看一下似乎很奇怪，两个几乎一模一样的`test`函数的汇编代码竟然不同！！！

事实上，侧面说明一个问题，静态局部变量实现的单例模式，影响到了编译器的优化分析，即带有`side effect`。

### 汇编对比结果汇总

1. `static local variable`比`local variable`运行时需要额外的指令开销
2. `static local variable`函数比`local variable`函数的内联机会小
3. `static local variable`难以触发像`imul`这样硬件提供的优化指令
4. `static local variable`将影响程序分析，迫使调用者无法进行更多的优化

*注：当然还有，像`movzx`这样的特殊指令对硬件可能也有比较大的负担，如无法并行流水线等*

### 运行性能

视频中运行100'000'000次的数据(macos下的valgrind装失败了，下载ing)：

执行指令数：801'875'746 <=> 351'875'745

执行总时长：19.94's <=> 3.62's

### 结论

1. **仅在真正需要惰性加载时，才使用`static local variable`**
2. **在确定已加载的场景下，或多处频繁调用，预先缓存**
3. **虽然从运行性能来看，单次执行耗时增加极少，但基于会影响程序分析和内联，有可能会导致蝴蝶效应。所以对于短小且调用分散的函数、或调用频次极高的函数，应尽量避免直接调用单例函数**

## 参考资料

[1] static members. [https://en.cppreference.com/w/cpp/language/static](https://en.cppreference.com/w/cpp/language/static)

[2] storage_duration#Static_local_variables. [https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables)
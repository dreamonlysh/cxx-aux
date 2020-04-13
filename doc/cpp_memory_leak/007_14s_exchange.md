> Video Link: [C++ Weekly - Ep 14 Standard Library Gems: `next` and `exchange`](https://www.youtube.com/watch?v=i3dK_ndZ72c&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=14)

## Optimization Case

https://gitee.com/harmonyos/OpenArkCompiler/blob/master/src/maple_ir/src/mir_lower.cpp

```c++
BlockNode *MIRLower::LowerBlock(BlockNode &block) {
  auto *newBlock = mirModule.CurFuncCodeMemPool()->New<BlockNode>();
  BlockNode *tmp = nullptr;
  if (block.GetFirst() == nullptr) {
    return newBlock;
  }
  StmtNode *nextStmt = block.GetFirst();
  ASSERT(nextStmt != nullptr, "nullptr check");
  do {
    StmtNode *stmt = nextStmt;
    nextStmt = stmt->GetNext();
    // ...
  } while (nextStmt != nullptr);
  return newBlock;
}
```

其中`do {} while`中

```c++
    StmtNode *stmt = nextStmt;
    nextStmt = stmt->GetNext();
```

可以替换为

```c++
    StmtNode *stmt = std::exchange(nextStmt, stmt->GetNext());
```

从实招来，替换之后个人是有些犹豫的，很不直观，改为`assign`或许更能接受一些。个人觉得最好的语法应该恰似注释中的表达：

```c++
// a, b = b, a;
std::swap(a, b);
// a, b = b, c;
a = std::exchange(b, c);
```

这个得看`c++`语言的演进了，那就再对比下，使用`std::exchange`能带来性能上的好处。`std::exchange`可能实现如下<sup>[1]</sup>：

```c++
template<class T, class U = T>
T exchange(T& obj, U&& new_value)
{
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}
```

与不使用`std::exchange`的方式相比，应该通常不会去写`std::move`，示例如下：

```c++
#include <utility>

int g_next = 0;
int next() __attribute__((noinline));
int next() {
  return g_next++;
}

int g_value;
int exchange_test(std::size_t count) __attribute__((noinline));
int exchange_test(std::size_t count) {
  int a = 0;
  for (std::size_t i = 0; i < count; i += 2) {
    int tmp = a;
    a = next();
    g_value += tmp + a;
  }
  return a;
}

int exchange_test2(std::size_t count) __attribute__((noinline));
int exchange_test2(std::size_t count) {
  int a = 0;
  for (std::size_t i = 0; i < count; i += 2) {
    int tmp = std::exchange(a, next());
    g_value += tmp + a;
  }
  return a;
}

std::size_t cnt;
int main(int argc, char* argv[]) {
  return exchange_test(cnt) + exchange_test2(cnt);
}
```

`gcc9.3 -std=c++17 -O2`生成汇编([godbolt](https://godbolt.org/))如下：

```assembly
exchange_test(unsigned long):
        test    rdi, rdi
        je      .L6
        xor     ecx, ecx
        xor     eax, eax
.L5:
        mov     esi, eax
        call    next()
        lea     edx, [rsi+1+rax]
        add     rcx, 2
        mov     DWORD PTR g_value[rip], edx
        cmp     rdi, rcx
        ja      .L5
        ret
.L6:
        xor     eax, eax
        ret
exchange_test2(unsigned long):
        xor     eax, eax
        test    rdi, rdi
        je      .L11
        xor     ecx, ecx
.L10:
        mov     esi, eax
        call    next()
        lea     edx, [rsi+1+rax]
        add     rcx, 2
        mov     DWORD PTR g_value[rip], edx
        cmp     rdi, rcx
        ja      .L10
        ret
.L11:
        ret
```

唯一的区别在于非`std::exchange`场景下，多了一条近期清零的指令`xor     eax, eax`。使用`clang10`指令数便一样多了。

## 结论

`std::exchange`表达方式并不非常清晰，性能上也没有明显的提升，且使用场景只有少数循环中需要备份历史，所以个人不是很推荐使用。

若上述场景中，`a`类型占用空间较大，需要显示`std::move`来提升性能，此时可以用`std::exchange`。或在泛型编程中，无法确定`a`类型时，使用`std::exchange`可以比较多的减少啰嗦的代码。

## 参考资料

[1] std::exchange. [https://zh.cppreference.com/w/cpp/utility/exchange](https://zh.cppreference.com/w/cpp/utility/exchange)
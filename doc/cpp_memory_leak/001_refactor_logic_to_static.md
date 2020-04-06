> Video Link: [C++ Weekly - Ep 1 ChaiScript_Parser Initialization Refactor](https://www.youtube.com/watch?v=EJtqHLvAIZE&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=1)

此视频的目标：将代码中具有初始化语义的代码进行重构，以此可以更多的利用编译器能力进行优化，从而获得更高的性能。

## Optimization Case

https://gitee.com/harmonyos/OpenArkCompiler/blob/master/src/maple_ir/src/lexer.cpp

```c++
void MIRLexer::GenName() {
  uint32 startIdx = curIdx;
  char c = GetNextCurrentCharWithUpperCheck();
  char cp = GetCharAt(curIdx - 1);
  if (c == '@' && (cp == 'h' || cp == 'f')) {
    // special pattern for exception handling labels: catch or finally
    c = GetNextCurrentCharWithUpperCheck();
  }
  while (utils::IsAlnum(c) || c < 0 || c == '_' || c == '$' || c == ';' ||
         c == '/' || c == '|' || c == '.' || c == '?' ||
         c == '@') {
    c = GetNextCurrentCharWithUpperCheck();
  }
  name = line.substr(startIdx, curIdx - startIdx);
}
```

如上代码，`while`循环中的判断表达式用以识别有效字符，判断式较多，且判断结构相似度高，将对其进行优化。首先将循环判断表达式提取并展开：

```c++
namespace {
inline bool validChar(char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         c < 0 ||
         c == '_' ||
         c == '$' ||
         c == ';' ||
         c == '/' ||
         c == '|' ||
         c == '.' ||
         c == '?' ||
         c == '@';
}
}
```

将匹配语义转换为查找，构建数据与算法分离的模型：

```c++
namespace {
inline bool validChar(char c) {
  static const std::set<char> chars = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', ..., 'z',
    'A', ..., 'Z',
    '_', '$', ';', '/', '|', '.', '?', '@'
  };
  return c < 0 || chars.find(c) != chars.end();
}
}
```

将待匹配的数据以`set`容器存储(`static chars`是因为此容器的数据是一开始就确定的，无需每次调用都构建)。不过没人会希望把具有范围的`char`再每个手动写一次，所以代码将会重构成：

```c++
inline std::set<char> genValidChars() {
  std::set<char> chars{
    '_', '$', ';', '/', '|', '.', '?', '@'
  };
  for (char c = '0'; c <= '9'; ++c) {
    chars.insert(c);
  }
  for (char c = 'a'; c <= 'z'; ++c) {
    chars.insert(c);
  }
  for (char c = 'A'; c <= 'Z'; ++c) {
    chars.insert(c);
  }
}

inline bool validChar(char c) {
  static const std::set<char> chars = genValidChars();
  return c < 0 || chars.find(c) != chars.end();
}
```

一般而言，到此种程度已经基本可以了，但从性能角度依然有优化空间。

1. `static`局部变量`C++`需保证其性能安全，所以其使得`validChar`性能依然不够极致
2. `char`具有有限的连续范围，可以对`set`进行优化

```c++
constexpr size_t kSpecialCharSize = 8;
constexpr std::array<bool, CHAR_MAX> genValidChars() {
  std::array<bool, CHAR_MAX> chars= {false};
  std::array<char, kSpecialCharSize> specialChars= {
    '_', '$', ';', '/', '|', '.', '?', '@'
  };
  for (size_t i = 0; i < kSpecialCharSize; ++i) {
    chars[specialChars[i]] = true;
  }
  for (char c = '0'; c <= '9'; ++c) {
    chars[c] = true;
  }
  for (char c = 'a'; c <= 'z'; ++c) {
    chars[c] = true;
  }
  for (char c = 'A'; c <= 'Z'; ++c) {
    chars[c] = true;
  }
  return chars;
}
constexpr std::array<bool, CHAR_MAX> chars = genValidChars();

inline bool validChar(char c) {
  return c < 0 || chars[c];
}
```

静态期计算出`chars`的结果，通过下面代码打印：

```c++
for (size_t i = 0; i < CHAR_MAX; ++i) {
  if (chars[i]) {
    std::cout << (char)i << " ";
  }
}
```

输出结果：

```
. / 0 1 2 3 4 5 6 7 8 9 ; ? @ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z _ a b c d e f g h i j k l m n o p q r s t u v w x y z | %*
```

在`g++ -std=c++17 -O2`的编译条件下，`validChar`函数的汇编只有简简单单的4条命令(在线汇编[godbolt](https://godbolt.org/))。

```assembly
        mov     edx, 1
        test    al, al
        js      .L2
        movzx   edx, BYTE PTR chars[rax]
```

而初始通过比较的方式生成了非常多的汇编指令：

```assembly
        mov     eax, edi
        mov     r8d, 1
        and     eax, -33
        sub     eax, 65
        cmp     al, 25
        jbe     .L1
        test    dil, dil
        js      .L1
        lea     eax, [rdi-36]
        cmp     al, 59
        jbe     .L3
        cmp     dil, 124
        jne     .L4
.L1:
        mov     eax, r8d
        ret
.L3:
        movabs  rdx, 576460752316004353
        bt      rdx, rax
        jc      .L1
.L4:
        lea     eax, [rdi-63]
        cmp     al, 1
        setbe   r8b
        cmp     dil, 46
        sete    al
        or      r8d, eax
        jmp     .L1
```

当然还可以做更进一步的优化，如将`c < 0`的场景也添加进`UCHAR_MAX`，通过一步偏移可压缩为2条指令。
## Default Initialization<sup>[1]</sup>

```c++
// Syntax
T object;

new T
new T() 		// (until C++03)
```

### 默认初始化场景

1. 具有`automatic`(栈上)、`static`、`thread-local`生命周期的对象未显示初始化
2. `new`创建的`dynamic`(堆上)生命周期的变量未显示初始化
3. 非静态成员变量、基类在构造函数初始化列表中未显示初始化

### 默认初始化行为

1. 若是`class`类型，则使用默认构造函数进行初始化
2. 若是`array`类型，则每个元素各自进行默认初始化
3. 其他情况，默认初始化什么也不做

然而比较具有迷惑性的是，例如像全局静态变量`static int i;`中`i`会被初始化为`0`，与以上行为并不一致。这里比较有意思，`c++`在概念上将初始化过程描述为两个阶段：`Zero initialization`<sup>[2]</sup>、`Other initializations`，此处`i`初始化为`0`是`Zero initialization`的结果，默认初始化什么也没做。

## 参考资料

[1] Default initialization. [https://en.cppreference.com/w/cpp/language/default_initialization](https://en.cppreference.com/w/cpp/language/default_initialization)

[2] Zero initialization. [https://en.cppreference.com/w/cpp/language/zero_initialization](https://en.cppreference.com/w/cpp/language/zero_initialization)



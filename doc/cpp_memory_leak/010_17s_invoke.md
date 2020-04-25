> Video Link: [C++ Weekly - Ep 17 C++17's `std::invoke`](https://www.youtube.com/watch?v=z-kUhwANrIw&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=17&pbjreload=10)

## Background

非静态成员函数与普通函数在使用函数指针进行调用的方式不同<sup>[1]</sup><sup>[2]</sup>。

普通函数指针：

```c++
int sum(int lhs, int rhs) {
  return lhs + rhs;
}

using binary_function = int (*)(int, int);

TEST(Invoke, GeneralFunction) {
  binary_function f = sum;
  ASSERT_EQ(f(1, 1), 2);
}

TEST(Invoke, GeneralFunctionPointer) {
  binary_function f = &sum;
  ASSERT_EQ((*f)(1, 1), 2);
}
```

非静态成员函数：

```c++
class SumObj {
public:
  int sum(int lhs, int rhs) {
    return (lhs + rhs) * scale;
  }
private:
  int scale = 100;
};

template <typename T>
using binary_method = int (T::*)(int, int);

TEST(Invoke, ClassMethod) {
  binary_method<SumObj> func = &SumObj::sum;
  SumObj obj;
  ASSERT_EQ((obj.*func)(1, 1), 200);
}
```

从上述两个例子可以看到函数指针的三个知识点：类型、定义、调用。其中非静态成员函数只支持`&`方式，而不支持`binary_method<SumObj> func = SumObj::sum;`。个人理解，取普通函数指针是地地道道的取函数地址，而非静态成员函数并不是孤立的函数，其中包含了与类型、与非静态成员函数之间的关系，所以讲非静态成员函数类似非静态成员变量，其物理存储可能是类型的偏移，只有通过`&`才计算出实际地址(仅作为理解的猜想)。

## Invoke Case

上述的使用方式并不是很友好，从分析看有两个麻烦：

1. `(T::*)`与`(obj.*func)`的声明和调用方式不友好(笔者亦是看到`invoke`才学会这种方式)
2. 两种不同的方式使需要同时支持非静态成员函数和普通函数的模块写多份代码支持

通常来说，对于非静态成员函数，如上例中`SumObj::sum`，我们会将其想象为：

```c++
int sum(SumObj* obj, int lhs, int rhs);
ASSERT_EQ(sum(obj, 1, 1), 200);
```

`std::invoke`恰好满足这样的表达，所以使用`std::invoke`改写上述代码为：

```c++
TEST(Invoke, GeneralFunction) {
  ASSERT_EQ(std::invoke(sum, 1, 1), 2);
}

TEST(Invoke, GeneralFunctionPointer) {
  ASSERT_EQ(std::invoke(&sum, 1, 1), 2);
}

TEST(Invoke, ClassMethod) {
  SumObj obj;
  ASSERT_EQ(std::invoke(&SumObj::sum, obj, 1, 1), 200);
}
```

上述的改造还是比较令人满意的，但最令人纠结的是似乎仅仅是上述的例子反而有些不伦不类，具体的使用场景可能在哪儿？

## 参考资料

[1] 恼人的函数指针(一)。[https://www.cnblogs.com/AnnieKim/archive/2011/11/20/2255813.html](https://www.cnblogs.com/AnnieKim/archive/2011/11/20/2255813.html)

[2] 恼人的函数指针(二)：指向类成员的指针。[https://www.cnblogs.com/AnnieKim/archive/2011/12/04/2275589.html](https://www.cnblogs.com/AnnieKim/archive/2011/12/04/2275589.html)

[3] std::invoke。[https://zh.cppreference.com/w/cpp/utility/functional/invoke](https://zh.cppreference.com/w/cpp/utility/functional/invoke)

[4] A proposal to add invoke function template (Revision 1)。[http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4169.html](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4169.html)
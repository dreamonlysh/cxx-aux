> Video Link: [C++ Weekly - Ep 22 C++17's Nested Namespaces and `std::clamp`](https://www.youtube.com/watch?v=p32owQE_5u0&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=23)

## Verification Case

嵌套命名空间是一个非常小的写法改进，如：

```c++
// Style1
namespace A { namespace B { namespace C {

}}}
```

或：

```c++
// Style2
namespace A {
namespace B {
namespace C {

}
}
}
```

或：

```c++
// Style3
namespace A {
  namespace B {
    namespace C {

    }
  }
}
```

可以写成：

```c++
namespace A::B::C {

}
```

较少`{}`的嵌套着实提升了代码的简洁性。但从个人的角度，依然不会选择这种写法：

1. 多数代码不会采用`Style3`的命名空间风格，`Style1`/`Style2`并不存在缩进的问题
2. 命名空间嵌套数量通常在1-3之间，少数会有4层嵌套，所以命名空间嵌套大多不复杂
3. 显示使用`namespace`更为醒目，以及方便关键字查找，且`::`通常不用于定义，而用于使用
4. 没有可观的收益，且增加额外的概念，不如丢弃

## 参考资料

[1] Namespaces. [https://en.cppreference.com/w/cpp/language/namespace](https://en.cppreference.com/w/cpp/language/namespace)
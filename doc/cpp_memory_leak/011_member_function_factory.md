[010_17s_invoke](https://gitee.com/dreamonlysh/tony/blob/master/doc/cpp_memory_leak/010_17s_invoke.md)提到的`std::invoke`提供了统一的非静态成员函数和普通函数的统一调用，但遗留个问题：使用场景。

回忆方舟编译器中`FunctionFactory`<sup>[1]</sup>，似乎还欠老罗一个`MemberFunctionFactory`。

## Optimization Case

最为显眼最有机会使上工厂模式进行重构的代码，通常有着排比似的`if...else if...`或`switch...case...`，先构建以下用例：

```c++
class OpCluster {
public:
  int plus(int lhs, int rhs) {
    return (lhs + rhs) * scale;
  }

  int minus(int lhs, int rhs) {
    return (lhs - rhs) * scale;
  }

  int star(int lhs, int rhs) {
    return (lhs * rhs) * scale;
  }

  int div(int lhs, int rhs) {
    return (lhs / rhs) * scale;
  }

  std::optional<int> calc(char c, int lhs, int rhs) {
    if (c == '+')
      return plus(lhs, rhs);
    else if (c == '-')
      return minus(lhs, rhs);
    else if (c == '*')
      return star(lhs, rhs);
    else if (c == '/')
      return div(lhs, rhs);
    return std::nullopt;
  }

private:
  int scale = 10;
};

TEST(MemberFunctionFactory, Plus) {
  ASSERT_EQ(OpCluster().calc('+', 1, 2).value_or(0), 30);
}

TEST(MemberFunctionFactory, Minus) {
  ASSERT_EQ(OpCluster().calc('-', 1, 2).value_or(0), -10);
}

TEST(MemberFunctionFactory, Star) {
  ASSERT_EQ(OpCluster().calc('*', 1, 2).value_or(0), 20);
}

TEST(MemberFunctionFactory, Div) {
  ASSERT_EQ(OpCluster().calc('/', 1, 2).value_or(0), 0);
}
```

使用字典优化掉排比似的`if...else if...`：

```c++
class OpCluster {
public:
  OpCluster() {
    holder['+'] = &OpCluster::plus;
    holder['-'] = &OpCluster::minus;
    holder['*'] = &OpCluster::star;
    holder['/'] = &OpCluster::div;
  }

  int plus(int lhs, int rhs) {
    return (lhs + rhs) * scale;
  }

  int minus(int lhs, int rhs) {
    return (lhs - rhs) * scale;
  }

  int star(int lhs, int rhs) {
    return (lhs * rhs) * scale;
  }

  int div(int lhs, int rhs) {
    return (lhs / rhs) * scale;
  }

  std::optional<int> calc(char c, int lhs, int rhs) {
    auto it = ops().find(c);
    if (it == ops().end())
      return std::nullopt;
    return std::invoke(it->second, this, lhs, rhs);
  }

private:
  using op_type = int (OpCluster::*)(int, int);
  std::map<char, op_type> holder;

  int scale = 10;
};
```

上述代码中的`holder`便是工厂物理存储的核心，个人不是很希望在我们业务模块中暴露、重复工厂构建、查询以及存储等信息，且工厂配置只需初始配置一次即可，且由于`std::invoke`需要`this`，所以考虑添加`MemberFunctionFactory`作为基类工厂模板，并拥有静态的`holder`：

```c++
template <typename T>
class MemberFunctionFactory {
protected:
  using op_type = int (T::*)(int, int);
  using CreatorHolder = std::map<char, op_type>;
  static CreatorHolder& ins() {
    static CreatorHolder holder = {
        {'+', &T::plus},
        {'-', &T::minus},
        {'*', &T::star},
        {'/', &T::div},
    };
    return holder;
  }

  std::optional<int> member_invoke(char c, int lhs, int rhs) {
    auto it = ins().find(c);
    if (it == ins().end())
      return std::nullopt;
    return std::invoke(it->second, static_cast<T*>(this), lhs, rhs);
  }
};

class OpCluster : public MemberFunctionFactory<OpCluster> {
public:
  int plus(int lhs, int rhs) {
    return (lhs + rhs) * scale;
  }

  int minus(int lhs, int rhs) {
    return (lhs - rhs) * scale;
  }

  int star(int lhs, int rhs) {
    return (lhs * rhs) * scale;
  }

  int div(int lhs, int rhs) {
    return (lhs / rhs) * scale;
  }

  std::optional<int> calc(char c, int lhs, int rhs) {
    return member_invoke(c, lhs, rhs);
  }

private:
  int scale = 10;
};
```

接下来考虑的事情便是将`MemberFunctionFactory`更加泛化，

1. 需要将`holder`中的`KeyType`以及`MemberFunctionType`由工厂的使用者`OpCluster`提供
2. 提供更为泛化的初始化配置接口
3. 提供更为泛化的调用接口

以上几种元素提供后，一个基本的工厂基本成型。

但，在此处需要额外考虑两点问题，

1. `MemberFunctionType`的参数及返回值类型可能为类的私有域，所以`MemberFunctionFactory`上扩展`MemberFunctionType`的模板参数会将类私有域的内容强制暴露
2. 单个类可能需要支持多个工厂，要么使用多继承，要么组合多个工厂

综合考虑之后(没忍住，一下子搞到头了，过程缺失)，将以上代码重构为：

```c++
namespace estd {

template <typename ConcreteT>
class MemberFunctionFactory {
public:
  template <typename KeyT, typename RetT, typename... Args>
  class FactoryType;

  template <typename FactoryT>
  static constexpr size_t initialize(
      typename std::initializer_list<std::pair<typename FactoryT::key_type,
                                               typename FactoryT::op_type>> init) {
    for (auto p : init)
      FactoryT::holder.emplace(std::move(p));
    return init.size();
  }

  template <typename FactoryT>
  static void initialize_once(
      typename std::initializer_list<std::pair<typename FactoryT::key_type,
                                               typename FactoryT::op_type>> init) {
    static auto dummy = initialize<FactoryT>(std::move(init));
    (void)dummy;
  }

  template <typename FactoryT, typename... Args>
  std::optional<std::enable_if_t<!std::is_void_v<typename FactoryT::ret_type>,
                                 typename FactoryT::ret_type>>
  invoke(typename FactoryT::key_type c, Args&&... args) {
    if (auto it = FactoryT::holder.find(c); it != FactoryT::holder.end()) {
      return std::invoke(it->second, static_cast<ConcreteT*>(this),
                         std::forward<Args>(args)...);
    }
    return std::nullopt;
  }

  template <typename FactoryT, typename... Args>
  std::enable_if_t<std::is_void_v<typename FactoryT::ret_type>>
  invoke(typename FactoryT::key_type c, Args&&... args) {
    if (auto it = FactoryT::holder.find(c); it != FactoryT::holder.end()) {
      std::invoke(it->second, static_cast<ConcreteT*>(this),
                  std::forward<Args>(args)...);
    }
  }
};

template <typename ConcreteT>
template <typename KeyT, typename RetT, typename... Args>
class MemberFunctionFactory<ConcreteT>::FactoryType {
private:
  friend MemberFunctionFactory<ConcreteT>;

  using key_type = KeyT;
  using ret_type = RetT;
  using op_type = RetT (ConcreteT::*)(Args...);

  using CreatorHolder = std::map<key_type, op_type>;
  static CreatorHolder holder;
};

template <typename ConcreteT>
template <typename KeyT, typename RetT, typename... Args>
typename MemberFunctionFactory<ConcreteT>::template FactoryType<KeyT, RetT, Args...>::CreatorHolder
    MemberFunctionFactory<ConcreteT>::template FactoryType<KeyT, RetT, Args...>::holder = {};

} // estd

class OpCluster : public estd::MemberFunctionFactory<OpCluster> {
  using factory_type = FactoryType<char, int, int, int>;
public:
  OpCluster() {
    initialize_once<factory_type>({
      {'+', &OpCluster::plus},
      {'-', &OpCluster::minus},
      {'*', &OpCluster::star},
      {'/', &OpCluster::div}
    });
  }
  int plus(int lhs, int rhs) {
    return (lhs + rhs) * scale;
  }

  int minus(int lhs, int rhs) {
    return (lhs - rhs) * scale;
  }

  int star(int lhs, int rhs) {
    return (lhs * rhs) * scale;
  }

  int div(int lhs, int rhs) {
    return (lhs / rhs) * scale;
  }

  std::optional<int> calc(char c, int lhs, int rhs) {
    return invoke<factory_type>(c, lhs, rhs);
  }

private:
  int scale = 10;
};
```

`MemberFunctionFactory`提供了工厂域，其中`FactoryType`为外部定义的工厂类型，可以定义多种不同的工厂类型；`initialize`提供初始化工厂配置的能力，`initialize_once`为了简化延迟加载的使用场景；`invoke`提供两个函数实现，有返回值和无返回值。对于工厂使用者，需要定义不同的`FactoryType`，并对各工厂进行初始化，调用时指定工厂类型，即可像`std::invoke`调用普通函数一样运行。

## 参考资料

[1] OpenArkCompiler factory.h。[https://gitee.com/harmonyos/OpenArkCompiler/blob/master/src/maple_util/include/factory.h](https://gitee.com/harmonyos/OpenArkCompiler/blob/master/src/maple_util/include/factory.h)
> Video Link: [C++ Weekly - Ep 24 C++17's Structured Bindings](https://www.youtube.com/watch?v=aBZlbb9sE-g&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=25)

## Verification Case

结构化绑定是一个比较有意思的功能，从某个角度来看，使用`{}`可以将数据打包成`tuple`/`array`/`class`等，结构化绑定便是使用`auto []`将以打包的数据展开。举个最常见的例子<sup>[1]</sup>：

```c++
int main() {
  std::set<std::string> myset;
  if (auto it = myset.insert("Hello"); it->second) 
    std::cout << "insert is successful. The value is " << std::quoted(*iter->first) << '\n';
  else
    std::cout << "The value " << std::quoted(*iter->first) << " already exists in the set\n";
}
```

使用结构化绑定具名`iter`、`success`，代码表述能力更强，也更为精炼。

```c++
int main() {
  std::set<std::string> myset;
  if (auto [iter, success] = myset.insert("Hello"); success) 
    std::cout << "insert is successful. The value is " << std::quoted(*iter) << '\n';
  else
    std::cout << "The value " << std::quoted(*iter) << " already exists in the set\n";
}
```

### 结构化绑定`array`

```c++
TEST(StructuredBindingDeclarationArray, Value) {
  int a[2] = {1, 2};
  auto [x, y] = a;
  ASSERT_EQ(x, 1);
  ASSERT_EQ(y, 2);
  ASSERT_NE(&x, a);
  ASSERT_NE(&y, std::next(a));
}
```

将数组中的每个元素具名绑定。上述为值绑定，引用绑定可以定义为`auto& [x, y] = a;`或万能引用`auto&& [x, y] = a;`，`const`引用绑定添加`const`修饰符即可，如`const auto& [x, y] = a;`。`[`前的所有修饰符便像是修饰`[]`中的每个元素一样，这也是通常性的思维。

### 结构化绑定`struct`/`tuple`/`pair`...

```c++
std::tuple<float&, int, char&&> baseline()；
TEST_F(StructuredBindingDeclarationTuple, RValue) {
  auto [x, y, z] = this->baseline();
  ASSERT_TRUE((std::is_lvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(x)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_rvalue_reference_v<decltype(y)>));
  ASSERT_FALSE((std::is_lvalue_reference_v<decltype(z)>));
  ASSERT_TRUE((std::is_rvalue_reference_v<decltype(z)>));
  ASSERT_EQ(&x, &tx);
  ASSERT_NE(&y, &ty);
  ASSERT_EQ(y, 2);
}
```

其中值类型绑定时，`const`以及引用特性与绑定`array`场景下完全一致。而引用类型(`lvalue`/`rvalue`)绑定时，`const`以及引用特性均无影响，即其类型在结构定义时确定，后续结构化绑定的所有修饰符均对其无效。如：`std::tuple<float&, int, char&&>`，无论是`auto [x, y, z]`、`auto& [x, y, z]`亦或是`const auto& [x, y, z]`，`x`的type始终为非`const`的左值引用，`z`的type始终为非`const`的右值引用，只有`y`会受影响(同结构化绑定`array`)。

注：完整的测试用例可以参考https://gitee.com/dreamonlysh/tony/blob/master/support/estd/unittest/structured_binding_declaration_test.cpp

## 参考资料

[1] Structured binding declaration. [https://en.cppreference.com/w/cpp/language/structured_binding](https://en.cppreference.com/w/cpp/language/structured_binding)
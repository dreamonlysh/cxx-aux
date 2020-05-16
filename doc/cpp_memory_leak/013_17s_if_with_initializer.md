> Video Link: [C++ Weekly - Ep 21 C++17's `if` and `switch` Init Statements](https://www.youtube.com/watch?v=AiXU5EuLZgc&list=PLs3KjaCtOwSZ2tbuV1hx8Xz-rFZTan2J1&index=22)

## 应用场景

首先看下其等效表达<sup>[1]</sup>：

```c++
{
  init_statement
  if constexpr(optional) ( condition )
    statement-true
  else
    statement-false
}
```

与普通的`if`几乎相同，唯一的区别是最外层的`{}`提供了一块私有域。即`init_statement`中定义的变量不会再对`if`外的域产生影响，并及时回收，即精准作用域。这曾经是需要通过显示`{}`、或`lambda`表达式、或抽取函数等方式才能达到相同效果。

**场景1：变量初始化行为仅对当前条件作用域有效，其生命期随条件块结束即可终结。**

曾几何时，当如下格式的代码：

```c++
if () {
} else if () {
} else {
}
afterCondition()；
```

在`else if`处需要一个初始化，才能进行继续进行判断，可能会写成：

```c++
if () {
afterCondition()；
}

init_if_condition;
if () {
} else {
}
afterCondition()；
```

亦或是：

```c++
init_if_condition_only_else_if_needed;
if () {
} else if () {
} else {
}
afterCondition()；
```

使用该特性即可简单的改为：

```c++
if () {
} else if (init_if_condition; ) {
} else {
}
afterCondition()；
```

更为直接的获得连贯简洁的表达，不用多次调用条件块后的任务，亦不用提供一个更大作用域的变量。

**场景2：`if else if`的连贯性被`else if`条件所需的变量定义所以破坏。**

除了上述两个场景，其他便是一个写书写上的改善，也能带来一定的效果，举例如下：

https://gitee.com/harmonyos/OpenArkCompiler/blob/master/src/maple_me/src/me_alias_class.cpp

```c++
// This phase performs alias analysis based on Steensgaard's algorithm and
// represent the resulting alias relationships in the Maple IR representation
bool MeAliasClass::HasWriteToStaticFinal() const {
  for (auto bIt = func.valid_begin(); bIt != func.valid_end(); ++bIt) {
    for (const auto &stmt : (*bIt)->GetStmtNodes()) {
      if (stmt.GetOpCode() == OP_dassign) {
        const auto &dassignNode = static_cast<const DassignNode&>(stmt);
        if (dassignNode.GetStIdx().IsGlobal()) {
          const MIRSymbol *sym = mirModule.CurFunction()->GetLocalOrGlobalSymbol(dassignNode.GetStIdx());
          if (sym->IsFinal()) {
            return true;
          }
        }
      }
    }
  }
  return false;
}
```

如上代码的缩进层次比较高，使用两个`if`来判断“全局Dassign”，使用`initializer if`进行改造：

```c++
// This phase performs alias analysis based on Steensgaard's algorithm and
// represent the resulting alias relationships in the Maple IR representation
bool MeAliasClass::HasWriteToStaticFinal() const {
  for (auto bIt = func.valid_begin(); bIt != func.valid_end(); ++bIt) {
    for (const auto &stmt : (*bIt)->GetStmtNodes()) {
      if (auto *dassignNode = safe_cast<const DassignNode>(stmt);
          dassignNode != nullptr && dassignNode->GetStIdx().IsGlobal()) {
        const MIRSymbol *sym = mirModule.CurFunction()->GetLocalOrGlobalSymbol(dassignNode.GetStIdx());
        if (sym->IsFinal()) {
          return true;
        }
      }
    }
  }
  return false;
}
```

将针对`dassignNode`的判断集中在一个`if`表达式中，同时也较少了一级缩进层次。

注：如果不进行显示`dassignNode != nullptr`判空处理，亦可以写成`if (auto *dassignNode = safe_cast<const DassignNode>(stmt) && dassignNode->GetStIdx().IsGlobal()) {`，但方舟编码规范强制要求指针判空需要显示与`nullptr`进行比较，禁隐式转换。

## 参考资料

[1] if statement. [https://en.cppreference.com/w/cpp/language/if](https://en.cppreference.com/w/cpp/language/if)
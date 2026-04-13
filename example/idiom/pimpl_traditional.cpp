// Basic pimpl with heap allocation (pimpl<T, 0>)

#include <estd/idiom/pimpl.h>
#include <iostream>
#include <string>

// ============================================
// widget.h
// ============================================
class Widget : public es::idiom::pimpl<Widget> {
public:
  explicit Widget(const std::string& name);

  void doSomething();
  const std::string& getName() const;
};

// ============================================
// widget.cpp
// ============================================
struct WidgetImpl {
  std::string name;
  int counter = 0;

  WidgetImpl(const std::string& n) : name(n) {}

  void doSomething() {
    ++counter;
    std::cout << name << " doing something (count: " << counter << ")\n";
  }
};

template <>
struct es::idiom::pimpl_traits<Widget> {
  using impl_type = WidgetImpl;
};

Widget::Widget(const std::string& name) : pimpl(name) {}

void Widget::doSomething() { pimpl_cast(this)->doSomething(); }

const std::string& Widget::getName() const { return pimpl_cast(this)->name; }

// ============================================
// main.cpp
// ============================================
int main() {
  std::cout << "=== pimpl<T, 0> - Heap Allocation ===\n\n";

  Widget w("MyWidget");
  w.doSomething();
  w.doSomething();

  std::cout << "\nName: " << w.getName() << "\n";
  std::cout << "sizeof(Widget) = " << sizeof(Widget)
            << " bytes (just a pointer)\n";

  return 0;
}

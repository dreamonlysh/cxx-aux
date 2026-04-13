// pimpl with inline storage (pimpl<T, N>)

#include <estd/idiom/pimpl.h>
#include <iostream>
#include <string>

// ============================================
// fast_widget.h
// ============================================
class FastWidget : public es::idiom::pimpl<FastWidget, 64> {
public:
  explicit FastWidget(const std::string& name);

  void doSomething();
  const std::string& getName() const;
};

// ============================================
// fast_widget.cpp
// ============================================
struct FastWidgetImpl {
  std::string name;
  int counter = 0;
  double data[4] = {1.0, 2.0, 3.0, 4.0};

  FastWidgetImpl(const std::string& n) : name(n) {}

  void doSomething() {
    ++counter;
    data[0] += 0.5;
    std::cout << name << " doing something (count: " << counter
              << ", data[0]=" << data[0] << ")\n";
  }
};

template <>
struct es::idiom::pimpl_traits<FastWidget> {
  using impl_type = FastWidgetImpl;
};

FastWidget::FastWidget(const std::string& name) : pimpl(name) {}

void FastWidget::doSomething() { pimpl_cast(this)->doSomething(); }

const std::string& FastWidget::getName() const {
  return pimpl_cast(this)->name;
}

// ============================================
// main.cpp
// ============================================
int main() {
  std::cout << "=== pimpl<T, N> - Inline Storage ===\n\n";

  std::cout << "sizeof(FastWidget) = " << sizeof(FastWidget) << " bytes\n";
  std::cout << "sizeof(FastWidgetImpl) = " << sizeof(FastWidgetImpl)
            << " bytes\n";
  std::cout << "Storage size = 64 bytes\n";
  std::cout << "No heap allocation!\n\n";

  FastWidget w("MyFastWidget");
  w.doSomething();
  w.doSomething();

  std::cout << "\nName: " << w.getName() << "\n";

  FastWidget arr[3] = {FastWidget("W1"), FastWidget("W2"), FastWidget("W3")};

  std::cout << "\nArray of widgets (contiguous memory):\n";
  std::cout << "  &arr[0] = " << &arr[0] << "\n";
  std::cout << "  &arr[1] = " << &arr[1] << " (+" << sizeof(FastWidget)
            << " bytes)\n";
  std::cout << "  &arr[2] = " << &arr[2] << " (+" << sizeof(FastWidget)
            << " bytes)\n";

  return 0;
}

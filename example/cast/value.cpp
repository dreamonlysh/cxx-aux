// value.h example: using Value<DummyT> base class for dynamic casting

#include <estd/cast/value.h>
#include <iostream>

// ============================================
// expr.h
// ============================================
struct ExprTag {};

class Expr : public es::Value<ExprTag> {
protected:
  Expr(unsigned uid) : es::Value<ExprTag>(uid) {}

public:
  // Non-virtual function in base class
  void printType() const { std::cout << "Expr\n"; }
};

// ============================================
// literal.h
// ============================================
class Literal : public Expr {
public:
  static constexpr unsigned CLASSID = 1;
  Literal(int v) : Expr(CLASSID), value(v) {}

  // API specific to Literal
  int getValue() const { return value; }
  void setValue(int v) { value = v; }

private:
  int value;
};

// ============================================
// add.h
// ============================================
class Add : public Expr {
public:
  static constexpr unsigned CLASSID = 2;
  Add(Expr* l, Expr* r) : Expr(CLASSID), left(l), right(r) {}

  // API specific to Add
  Expr* getLeft() const { return left; }
  Expr* getRight() const { return right; }

private:
  Expr* left;
  Expr* right;
};

// ============================================
// main.cpp
// ============================================
int main() {
  std::cout << "=== Value Example ===\n\n";

  Literal a(2);
  Literal b(3);
  Add add(&a, &b);

  Expr& expr = add;

  // Base class method (non-virtual)
  expr.printType();

  // isa() - check type
  std::cout << "is Add? " << (es::isa<Add>(expr) ? "yes" : "no") << "\n";
  std::cout << "is Literal? " << (es::isa<Literal>(expr) ? "yes" : "no")
            << "\n";

  // cast() to access derived-specific API
  Add& addRef = es::cast<Add>(expr);
  std::cout << "Add has left and right children\n";

  // dyn_cast() for safe access to derived API
  if (Literal* lit = es::dyn_cast<Literal>(addRef.getLeft())) {
    std::cout << "Left child is Literal with value: " << lit->getValue()
              << "\n";
    lit->setValue(10);
    std::cout << "Changed to: " << lit->getValue() << "\n";
  }

  return 0;
}

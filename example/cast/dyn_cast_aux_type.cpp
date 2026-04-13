// dyn_cast_aux_type example: wrapper class with intermediate type for casting

#include <estd/cast/dyn_cast.h>
#include <iostream>

// ============================================
// shape.h - base hierarchy
// ============================================
enum class ShapeType { Circle, Rectangle };

class Shape {
public:
  virtual ~Shape() = default;
  virtual ShapeType type() const = 0;
};

class Circle : public Shape {
public:
  double radius;
  Circle(double r) : radius(r) {}
  ShapeType type() const override { return ShapeType::Circle; }
};

class Rectangle : public Shape {
public:
  double width, height;
  Rectangle(double w, double h) : width(w), height(h) {}
  ShapeType type() const override { return ShapeType::Rectangle; }
};

template <>
struct es::isa_traits<Circle, Shape> {
  static bool doit(const Shape& s) { return s.type() == ShapeType::Circle; }
};

template <>
struct es::isa_traits<Rectangle, Shape> {
  static bool doit(const Shape& s) { return s.type() == ShapeType::Rectangle; }
};

template <>
struct es::cast_traits<Circle, Shape> {
  static Circle& doit(Shape& s) { return static_cast<Circle&>(s); }
  static const Circle& doit(const Shape& s) {
    return static_cast<const Circle&>(s);
  }
};

template <>
struct es::cast_traits<Rectangle, Shape> {
  static Rectangle& doit(Shape& s) { return static_cast<Rectangle&>(s); }
  static const Rectangle& doit(const Shape& s) {
    return static_cast<const Rectangle&>(s);
  }
};

// ============================================
// shape_ref.h - wrapper with dyn_cast_aux_type
// ============================================
class ShapeRef {
public:
  // This tells dyn_cast to use Shape as the intermediate type
  using dyn_cast_aux_type = Shape;

  ShapeRef(Shape& s) : shape(&s) {}
  Shape& get() { return *shape; }
  const Shape& get() const { return *shape; }

  // Forward to Shape for isa_traits
  operator Shape&() { return *shape; }
  operator const Shape&() const { return *shape; }

private:
  Shape* shape;
};

// ============================================
// main.cpp
// ============================================
int main() {
  std::cout << "=== dyn_cast_aux_type Example ===\n\n";

  Circle c(2.0);
  Rectangle r(3.0, 4.0);

  ShapeRef ref1(c);
  ShapeRef ref2(r);

  // isa() uses dyn_cast_aux_type = Shape internally
  std::cout << "ref1 is Circle? " << (es::isa<Circle>(ref1) ? "yes" : "no")
            << "\n";
  std::cout << "ref2 is Rectangle? "
            << (es::isa<Rectangle>(ref2) ? "yes" : "no") << "\n";

  // cast() also uses dyn_cast_aux_type
  std::cout << "Circle radius: " << es::cast<Circle>(ref1).radius << "\n";

  // dyn_cast() works through the wrapper
  if (Circle* circle = es::dyn_cast<Circle>(&ref1)) {
    std::cout << "ref1 wraps a Circle\n";
  }

  if (Circle* circle = es::dyn_cast<Circle>(&ref2)) {
    std::cout << "ref2 wraps a Circle\n";
  } else {
    std::cout << "ref2 does not wrap a Circle\n";
  }

  return 0;
}

// dyn_cast example: custom type hierarchy with isa_traits/cast_traits

#include <estd/cast/dyn_cast.h>
#include <iostream>

// ============================================
// shape.h
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
// main.cpp
// ============================================
int main() {
  std::cout << "=== dyn_cast Example ===\n\n";

  Circle c(2.0);
  Rectangle r(3.0, 4.0);

  Shape& s1 = c;
  Shape& s2 = r;

  // isa() - check type
  std::cout << "s1 is Circle? " << (es::isa<Circle>(s1) ? "yes" : "no") << "\n";
  std::cout << "s1 is Rectangle? " << (es::isa<Rectangle>(s1) ? "yes" : "no")
            << "\n";

  // cast() - unchecked cast
  std::cout << "Circle radius: " << es::cast<Circle>(s1).radius << "\n";

  // dyn_cast() - safe cast (pointer returns nullptr, reference throws)
  if (Circle* circle = es::dyn_cast<Circle>(&s2)) {
    std::cout << "s2 is a circle\n";
  } else {
    std::cout << "s2 is not a circle\n";
  }

  return 0;
}

// type_id.h example: getting unique runtime type identifiers

#include <estd/cast/type_id.h>
#include <iostream>

struct A {};
struct B {};
struct C {};

int main() {
  std::cout << "=== type_id Example ===\n\n";

  std::cout << "1. Basic usage - different types have different IDs:\n";
  size_t id_a = es::type_id<A>();
  size_t id_b = es::type_id<B>();
  size_t id_c = es::type_id<C>();

  std::cout << "type_id<A>() = " << id_a << "\n";
  std::cout << "type_id<B>() = " << id_b << "\n";
  std::cout << "type_id<C>() = " << id_c << "\n";
  std::cout << "All IDs are unique: "
            << (id_a != id_b && id_b != id_c && id_a != id_c ? "yes" : "no")
            << "\n\n";

  std::cout << "2. Same type always returns the same ID:\n";
  size_t id_a2 = es::type_id<A>();
  std::cout << "type_id<A>() called again = " << id_a2 << "\n";
  std::cout << "IDs match: " << (id_a == id_a2 ? "yes" : "no") << "\n\n";

  std::cout << "3. Type deduction from value:\n";
  A a;
  B b;
  size_t id_from_value_a = es::type_id(a);
  size_t id_from_value_b = es::type_id(b);
  std::cout << "type_id(a) = " << id_from_value_a << "\n";
  std::cout << "type_id(b) = " << id_from_value_b << "\n";
  std::cout << "Matches explicit call: "
            << (id_from_value_a == id_a && id_from_value_b == id_b ? "yes"
                                                                   : "no")
            << "\n\n";

  std::cout << "4. CV-qualifiers are ignored:\n";
  size_t id_const_a = es::type_id<const A>();
  size_t id_volatile_a = es::type_id<volatile A>();
  size_t id_const_volatile_a = es::type_id<const volatile A>();
  std::cout << "type_id<const A>() = " << id_const_a << "\n";
  std::cout << "type_id<volatile A>() = " << id_volatile_a << "\n";
  std::cout << "type_id<const volatile A>() = " << id_const_volatile_a << "\n";
  std::cout << "All equal to type_id<A>(): "
            << (id_const_a == id_a && id_volatile_a == id_a &&
                        id_const_volatile_a == id_a
                    ? "yes"
                    : "no")
            << "\n\n";

  std::cout << "5. Using type_id for type comparison in containers:\n";
  struct TypeInfo {
    size_t id;
    const char* name;
  };
  TypeInfo types[] = {{es::type_id<A>(), "A"},
                      {es::type_id<B>(), "B"},
                      {es::type_id<C>(), "C"}};

  A obj;
  size_t obj_id = es::type_id(obj);
  for (const auto& t : types) {
    if (t.id == obj_id) {
      std::cout << "Object is of type: " << t.name << "\n";
      break;
    }
  }

  return 0;
}

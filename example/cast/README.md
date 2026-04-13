# Cast Examples

This directory contains examples demonstrating the `es::cast` library.

## Examples

### 1. dyn_cast.cpp
**Manual isa_traits/cast_traits specialization**

For custom type hierarchies where you control the type checking logic.

```cpp
// Define your hierarchy
class Shape {
public:
    virtual ShapeType type() const = 0;
};

class Circle : public Shape { /* ... */ };

// Specialize isa_traits for type checking
template <>
struct es::isa_traits<Circle, Shape> {
    static bool doit(const Shape& s) {
        return s.type() == ShapeType::Circle;
    }
};

// Specialize cast_traits for casting
template <>
struct es::cast_traits<Circle, Shape> {
    static Circle& doit(Shape& s) {
        return static_cast<Circle&>(s);
    }
};

// Usage
Shape& s = getShape();
if (es::isa<Circle>(s)) {
    Circle& c = es::cast<Circle>(s);
}
```

**Compile:**
```bash
g++ -std=c++17 -I include example/cast/dyn_cast.cpp -o dyn_cast
./dyn_cast
```

### 2. value.cpp
**Value<DummyT> base class pattern**

Automatic isa_traits/cast_traits via CLASSID.

```cpp
struct ExprTag {}; // Unique tag for hierarchy

class Expr : public es::Value<ExprTag> {
protected:
    Expr(unsigned uid) : es::Value<ExprTag>(uid) {}
};

class Literal : public Expr {
public:
    static constexpr unsigned CLASSID = 1;  // Required
    Literal(int v) : Expr(CLASSID), value(v) {}
    int value;
};

// Usage - no traits specialization needed
Expr& e = getExpr();
if (es::isa<Literal>(e)) {
    Literal& lit = es::cast<Literal>(e);
}
```

**Key Points:**
- Inherit from `Value<UniqueTag>`
- Define `static constexpr unsigned CLASSID` in each derived class
- Pass CLASSID to Value constructor
- Automatic traits specialization

**Compile:**
```bash
g++ -std=c++17 -I include example/cast/value.cpp -o value
./value
```

### 3. dyn_cast_aux_type.cpp
**Wrapper with dyn_cast_aux_type**

For wrapper types that hold a reference to a castable type.

```cpp
class ShapeRef {
public:
    using dyn_cast_aux_type = Shape;  // Route through Shape

    ShapeRef(Shape& s) : shape(&s) {}
    operator Shape&() { return *shape; }

private:
    Shape* shape;
};

// Usage - dyn_cast works through wrapper
ShapeRef ref = getShapeRef();
if (Circle* c = es::dyn_cast<Circle>(&ref)) {
    // Successfully cast through wrapper
}
```

**Key Points:**
- Define `using dyn_cast_aux_type = BaseType;` in wrapper
- Provides implicit conversion to BaseType
- Reuses existing traits for BaseType

**Compile:**
```bash
g++ -std=c++17 -I include example/cast/dyn_cast_aux_type.cpp -o dyn_cast_aux_type
./dyn_cast_aux_type
```

### 4. type_id.cpp
**Unique runtime type identifiers**

Get a unique identifier for each type at runtime.

```cpp
struct A {};
struct B {};

size_t id_a = es::type_id<A>();
size_t id_b = es::type_id<B>();
assert(id_a != id_b);

// Type deduction from value
A a;
size_t id = es::type_id(a);  // Same as type_id<A>()

// CV-qualifiers are ignored
assert(es::type_id<A>() == es::type_id<const A>());
```

**Key Points:**
- Returns unique `size_t` for each distinct type
- Only works for class types (struct/class)
- CV-qualifiers (const/volatile) are ignored
- ID is stable within a single program execution

**Compile:**
```bash
g++ -std=c++17 -I include example/cast/type_id.cpp -o type_id
./type_id
```

## API Summary

### isa<T>(obj)
Check if object can be cast to type T.

```cpp
bool isa<T>(U& obj);    // Reference version
bool isa<T>(U* obj);    // Pointer version (returns false for null)
```

### cast<T>(obj)
Unchecked cast to type T. Caller must ensure `isa<T>(obj)` is true.

```cpp
T& cast<T>(U& obj);     // Reference version
T* cast<T>(U* obj);     // Pointer version
```

### dyn_cast<T>(obj)
Safe cast - returns nullptr for pointers, throws for references on failure.

```cpp
T* dyn_cast<T>(U* obj);           // Returns nullptr on failure
T& dyn_cast<T>(U& obj);           // Throws std::runtime_error on failure
```

## Comparison

| Feature | isa | cast | dyn_cast |
|---------|-----|------|----------|
| Type check | Yes | No | Yes |
| Returns | bool | T&/T* | T* (ptr) or T& (ref) |
| Safety | Safe | Unsafe | Safe |
| Null handling | N/A | UB | Returns nullptr (ptr) |

## Notes

These examples depend on the `estd::cast` library and must be compiled with the project's include path.

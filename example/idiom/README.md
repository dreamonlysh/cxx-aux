# pimpl Idiom Examples

This directory contains simple examples demonstrating the `estd::idiom::pimpl` library.

## Examples

### 1. pimpl_traditional.cpp
**pimpl<T, 0> - Heap Allocation**

The simplest form of pimpl using heap allocation.

```cpp
class Widget : public es::idiom::pimpl<Widget> {  // pimpl<Widget, 0>
public:
    explicit Widget(const std::string& name);
    void doSomething();
};

// In .cpp file:
struct WidgetImpl {  // Separate implementation type
    std::string name;
    int counter = 0;
};

template <>
struct es::idiom::pimpl_traits<Widget> {
    using impl_type = WidgetImpl;  // Maps Widget to WidgetImpl
};
```

**Compile:**
```bash
cd /path/to/cxx-aux
g++ -std=c++17 -I include example/idiom/pimpl_traditional.cpp -o pimpl_traditional
./pimpl_traditional
```

### 2. pimpl_inline_storage.cpp
**pimpl<T, N> - Inline Storage**

pimpl with inline storage to avoid heap allocation.

```cpp
class FastWidget : public es::idiom::pimpl<FastWidget, 64> {  // 64 bytes inline
public:
    explicit FastWidget(const std::string& name);
    void doSomething();
};

// In .cpp file:
struct FastWidgetImpl {
    std::string name;
    int counter = 0;
};

template <>
struct es::idiom::pimpl_traits<FastWidget> {
    using impl_type = FastWidgetImpl;
};
```

**Benefits:**
- No heap allocation
- Better cache locality
- Contiguous memory for arrays

**Compile:**
```bash
g++ -std=c++17 -I include example/idiom/pimpl_inline_storage.cpp -o pimpl_inline_storage
./pimpl_inline_storage
```

### 3. pimpl_multiple_inheritance.cpp
**pimpl with Multiple Inheritance using pimpl_type**

A class that inherits from multiple pimpl bases.

**File structure:**
- `logger.h` - Logger class declaration
- `service.h` - Service class declaration
- `logger.cpp` - LoggerImpl and Logger implementation
- `service.cpp` - ServiceImpl and Service implementation

```cpp
// logger.h
class Logger : public es::idiom::pimpl<Logger, 64> {
public:
    explicit Logger(const std::string& name);
    void log(const std::string& message);
};

// service.h
class Service : public es::idiom::pimpl<Service, 64>, public Logger {
public:
    explicit Service(const std::string& name);
    void run();
};

// service.cpp
struct ServiceImpl {
    std::string serviceName;
};

template <>
struct es::idiom::pimpl_traits<Service> {
    using impl_type = ServiceImpl;
    using pimpl_type = es::idiom::pimpl<Service, 64>;
};

Service::Service(const std::string& name) 
    : es::idiom::pimpl<Service, 64>(name), Logger(name)
{}
```

**Key Points:**
- Logger and Service each have their own pimpl storage
- `pimpl_type = pimpl<Service, 64>` routes pimpl_cast through Service's pimpl base
- Constructor must qualify `pimpl<Service, 64>(name)` to disambiguate

**Compile:**
```bash
g++ -std=c++17 -I include example/idiom/pimpl_multiple_inheritance.cpp -o pimpl_multiple_inheritance
./pimpl_multiple_inheritance
```

## API Summary

### pimpl<T, 0> - Heap Allocation
```cpp
class MyClass : public es::idiom::pimpl<MyClass> {
    // sizeof(MyClass) = sizeof(void*)
};
```

### pimpl<T, N> - Inline Storage
```cpp
class MyClass : public es::idiom::pimpl<MyClass, 64> {
    // sizeof(MyClass) = 64 bytes
    // sizeof(MyClassImpl) must be <= 64
};
```

### pimpl_traits
```cpp
// In .cpp file:
struct WidgetImpl {
    std::string name;
    int counter = 0;
};

template <>
struct es::idiom::pimpl_traits<Widget> {
    using impl_type = WidgetImpl;                      // Required: maps Widget to WidgetImpl
    using pimpl_type = es::idiom::pimpl<Widget, 64>;  // Optional: for multiple pimpl inheritance
};
```

### pimpl_cast
```cpp
void Widget::doSomething() {
    pimpl_cast(this)->counter++;  // Access WidgetImpl
    pimpl_cast(this)->name = "updated";
}
```

## Comparison

| Feature | pimpl<T, 0> | pimpl<T, N> |
|---------|-------------|-------------|
| Allocation | Heap | Inline |
| Object Size | Small (ptr) | Large (N bytes) |
| Cache Locality | Poor | Good |
| Flexibility | High | Medium |

## Notes

These examples depend on the `estd::idiom::pimpl` library and must be compiled with the project's include path.

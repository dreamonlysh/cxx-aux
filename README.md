# cxx-aux

# Description

Commonly used lite libraries and C++ standard library extensions for guyuemeng.

- include/estd

The `estd` is short for extension of cpp standard, it includes some general header only apis that mostly may have relation with the cpp std.

- include/cxxaux

Here contains some small modules that may not be so basic as the cpp std, but are general parts to generate a tool.

# estd

## meta.h

Apis for meta programing, there lool up to the type_traits in std.

Examples:

- Generate apis of verify class member

```c++
/// @brief has_member_begin, has_member_begin_v
META_HAS_MEMBER_FUNCTION(begin);
/// @brief has_member_end, has_member_end_v
META_HAS_MEMBER_FUNCTION(end);

/// @brief whether a container can be visited by begin and end
/// @tparam T a container
template <typename T>
struct is_iterable
    : std::bool_constant<has_member_begin_v<T> && has_member_end_v<T>> {};

/// @brief whether a container can be visited by begin and end
/// @tparam T a container
template <typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;
```

## bit.h

Apis for bit level operation, they are originated from the book **Hacker's Delight** and expanded when develop a llvm tool from an AI instruction set architecture.

Examples:

- Count the number of bit 1

```c++
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr unsigned count_bit1(T v) noexcept;
```

- Verify whether a number is power of 2

```c++
template <typename T, typename = std::enable_if_t<std::is_unsigned_v<T>>>
constexpr bool is_pow2(T v) noexcept;
```

## integer.h

Type strict integer wrapper, it tries to make an integer type meaningful and different type can not be calculated simply.

Examples:

- Differ bit number and byte number

```c++
class __BitTag;
using Bit = Integer<uint32_t, __BitTag>;
constexpr Bit operator""_b(unsigned long long int v) {
  return Bit{static_cast<uint32_t>(v)};
}

class __ByteTag;
using Byte = Integer<uint32_t, __ByteTag>;
constexpr Byte operator""_B(unsigned long long int v) {
  return Byte{static_cast<uint32_t>(v)};
}
```

## memory.h

Memory manager apis, there are mostly the memory pool support apis till now.

- layout_bit_mapping

Layout for a continuous memory to be designed as multi sub memory with bit flags to tell whether the memory is use or not.

```
/// For the hierarchy is one, the memory will be designed as
///     header                 data             tail
/// m1: | bits_of<MappingType> | {n}MemberBytes | ? |
/// tips: the tail unmapping is small than on MemberBytes that can not alloc
///
/// For the hierarchy is two, the memory will be designed as
///     1st header             2st headers               data             tail
/// m1: | bits_of<MappingType> | {m}bits_of<MappingType> | {n}MemberBytes | ? |
/// tips: the 1st header is a mapping to 2st headers
```

- layout_stack

Layout for multi memory blocks, with the first ptr size memory used as linker to previous.

```
///     header   data
/// m1: |  null  |  {x}B  |
/// m2: |  &m1   |  {y}B  |
/// m3: |  &m2   |  {z}B  |
```

- memory_pool_uniform

Memory pool for the types with the same size, the layout_bit_mapping and layout_stack are used for different type size.

- object_pool

Object pool for the same objects based on memory_pool_uniform. sub_object_pool is designed for perform in multi thread.

## flat_vector

A vector container wrapper use an array as the base memory.

## cast/

A cast facility with `isa`/`cast`/`dyn_cast` apis to replace the `dynamic_cast` which is not efficient with RTTI.

Examples:

```c++
class A : public es::Value<void> {
public:
  static constexpr unsigned CLASSID = 1;

  A() : es::Value<void>(CLASSID) {}

  int v = 1;
};

class B : public es::Value<void> {
public:
  static constexpr unsigned CLASSID = 2;

  B() : es::Value<void>(CLASSID) {}

  int v = 2;
};

TEST(CastTest, tests) {
  A a;
  es::Value<void>& ar = a;
  ASSERT_TRUE(es::isa<A>(ar));
  ASSERT_FALSE(es::isa<B>(ar));
  
  ASSERT_EQ(es::cast<A>(ar).v, 1);
  
  auto* pa = es::dyn_cast<A>(&ar);
  ASSERT_NE(pa, nullptr);
  ASSERT_EQ(pa->v, 1);
}
```

## string/

Facilities for string.

- algorithm

Algorithm for string append and replace.

- flat_string

A string container wrapper use an array as the base memory.

- string_table

A table to store strings, it is designed for std::string_view that the memory may be released when kept as a member.

## idiom/

Idioms of c++, there is only a pimpl idiom till now.

- pimpl

Wrapper for the traditional pimpl idiom, and provides an optimized mode of the memory allocation.

Example:

```c++
// a.h
class A : public es::idiom::Pimpl<A, 64> {
public:
  A(std::ostringstream& oss, int delta);
  ~A() noexcept;

  int add(int lhs, int rhs);
};

// a.cpp
class AImpl {
public:
  AImpl(std::ostringstream& oss, int delta) : oss(oss), delta(delta) {
    oss << "AImpl";
  }

  ~AImpl() noexcept { oss << "~AImpl"; }

  int add(int lhs, int rhs) { return lhs + rhs + delta; }

private:
  std::ostringstream& oss;
  int delta;
};

template <>
struct es::idiom::pimpl_traits<A> {
  using impl_type = AImpl;
  using pimpl_type = es::idiom::Pimpl<A, 64>; // optional
};

A::A(std::ostringstream& oss, int delta)
    : es::idiom::Pimpl<A, 64>(oss, delta) {}
A::~A() noexcept = default;

int A::add(int lhs, int rhs) { return pimpl_cast(this)->add(lhs, rhs); }

TEST(PimplTest, WithStorageEmbed) {
  std::ostringstream oss;
  {
    A a(oss, 10);
    ASSERT_EQ(oss.str(), "AImpl");
    oss.str("");
    ASSERT_EQ(a.add(1, 2), 13);
  }

  ASSERT_EQ(oss.str(), "~AImpl");
}
```

# cxxaux

## binary/binary_decoder

A `std::ifstream` like apis to decode data in a memory.

## binary/elf/

A simple apis to parse binary like sections from an elf format binary.

## utility/logger_wrapper

An unified logger apis defined as a wrapper to reduce the complex of using different third logger modules. Currently there are wrappers for std::stream and the spdlog.

## utility/memory_pool

A simple memory pool for some simple usage.


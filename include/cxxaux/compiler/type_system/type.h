// Copyright (c) 2023 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
//             http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef COMPILER_TYPE_SYSTEM_TYPE_H
#define COMPILER_TYPE_SYSTEM_TYPE_H
#include <cstdint>
#include <vector>

namespace bin { namespace type {

enum TypeID : uint32_t {
  TID_UNKNOWN,
  TID_INTEGER,
  TID_FLOATING_POINT,
  TID_POINTER,
  TID_ARRAY,
  TID_STRUCT
};

class Type {
public:
  explicit Type(uint32_t id) : id_(id) {}
  explicit Type(uint32_t id, uint32_t size) : id_(id), size_(size) {}
  virtual ~Type() noexcept = default;

  uint32_t id() const { return id_; }

  uint32_t size() const { return size_; }

  void setSize(uint32_t s) { size_ = s; }

private:
  uint32_t id_ = TID_UNKNOWN;
  uint32_t size_ = 0;
};

class Integer final : public Type {
  enum Flags { FLAG_SIGNED = 0x1, FLAG_CONST = 0x1 << 1 };

public:
  Integer(bool isSigned, bool isConst) : Type(TID_INTEGER) {
    flags_ |= isSigned ? FLAG_SIGNED : 0;
    flags_ |= isConst ? FLAG_CONST : 0;
  }
  ~Integer() noexcept = default;

  bool isSigned() const { return flags_ & FLAG_SIGNED; }

  bool isConst() const { return flags_ & FLAG_CONST; }

private:
  uint32_t flags_ = 0;
};

class FloatingPoint final : public Type {
public:
  FloatingPoint() : Type(TID_FLOATING_POINT) {}
  ~FloatingPoint() noexcept = default;
};

class Pointer final : public Type {
public:
  Pointer() : Type(TID_POINTER, sizeof(void*)) {}
  ~Pointer() noexcept = default;
};

class Array final : public Type {
public:
  explicit Array(const Type* typ) : Type(TID_ARRAY), elemTyp_(typ) {}
  ~Array() noexcept = default;

  const Type* element() const { return elemTyp_; }

private:
  const Type* elemTyp_ = nullptr;
};

class Structure final : public Type {
public:
  Structure() : Type(TID_STRUCT) {}
  ~Structure() noexcept = default;

  uint32_t align() const;

  size_t fieldCount() const { return fields_.size(); }

  const Type* fieldAt(size_t i) const { return fields_[i]; }

private:
  std::vector<const Type*> fields_;
};
}} // namespace bin::type
#endif // COMPILER_TYPE_SYSTEM_TYPE_H

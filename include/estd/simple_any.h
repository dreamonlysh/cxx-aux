#ifndef TONY_SIMPLE_ANY_H
#define TONY_SIMPLE_ANY_H

namespace es {

class simple_any {
  // The implement of gcc use the size of `void*` as the default buffer.
  // And llvm triple the size of `void*`.
  using __Buffer = std::aligned_storage_t<sizeof(void*), std::alignment_of<void*>::value>;

  // The so called small talk.
  // If the data to store no more than the __Storage size, use buf_
  // If the data to store out of the __Storage size, new the object on ptr_
  union __Storage {
    void* ptr_;
    __Buffer buf_; // char[8] size of memory.
  };

  // The handler is the polymorphism with different operation for the small talk.
  enum class _Action { _Destroy, _Copy, _Move, _Get, _TypeInfo };
  using __Handler = void* (*)(_Action, simple_any const *, simple_any *);
public:
  // public interface
private:
  // 1. Operations on __Storage with different type
  // 2. hl_ == nullptr to differ if there is valid data.
//  __Handler hl_;
//  __Storage st_;
};

template <typename ValueType>
ValueType* simple_any_cast(simple_any&) {
  // [optional] The __Handler with different address(template with type) for different type.
  // [optional] use type info
  // [optional] template <typename ValueType> struct unique_id {static auto value = 0;}; the address of value is unique type id.
  return nullptr;
}

}// namespace es

#endif//TONY_SIMPLE_ANY_H

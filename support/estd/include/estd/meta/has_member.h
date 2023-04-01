#ifndef ES_HAS_MEMBER_H
#define ES_HAS_MEMBER_H
#include <type_traits>

namespace es {

#define META_HAS_MEMBER(name, cond)                                            \
  template <typename T, typename = std::void_t<>>                              \
  struct has_member_##name : std::false_type {};                               \
  template <typename T>                                                        \
  struct has_member_##name<T, std::void_t<cond>> : std::true_type {};          \
  template <typename T>                                                        \
  constexpr bool has_member_##name##_v = has_member_##name<T>::value

#define META_HAS_MEMBER_TYPE(type) META_HAS_MEMBER(type, typename T::type)
#define META_HAS_MEMBER_DATA(data)                                             \
  META_HAS_MEMBER(data, decltype(std::declval<T>().data))
#define META_HAS_MEMBER_FUNCTION(func)                                         \
  META_HAS_MEMBER(func, decltype(std::declval<T>().func()))

// has_member_iterator, has_member_iterator_v
META_HAS_MEMBER_TYPE(iterator);

// has_member_begin, has_member_begin_v, has_member_end, has_member_end_v
META_HAS_MEMBER_FUNCTION(begin);
META_HAS_MEMBER_FUNCTION(end);

}// namespace es

#endif//ES_HAS_MEMBER_H

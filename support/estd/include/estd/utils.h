#ifndef ESTD_UTILS_H
#define ESTD_UTILS_H

namespace es {

template <typename T>
constexpr bool not_null(T* p) {
  return p != nullptr;
}

struct disabled_copy_move {
  disabled_copy_move() = default;
  disabled_copy_move(const disabled_copy_move&) = delete;
  disabled_copy_move& operator=(const disabled_copy_move&) = delete;
  disabled_copy_move(disabled_copy_move&&) = delete;
  disabled_copy_move& operator=(disabled_copy_move&&) = delete;
};

}
#endif //ESTD_UTILS_H

#ifndef ESTD_UTILS_H
#define ESTD_UTILS_H

namespace es {

struct disabled_copy {
  disabled_copy() = default;
  disabled_copy(const disabled_copy&) = delete;
  disabled_copy& operator=(const disabled_copy&) = delete;
  disabled_copy(disabled_copy&&) = default;
  disabled_copy& operator=(disabled_copy&&) = default;
};

struct disabled_move {
  disabled_move() = default;
  disabled_move(const disabled_move&) = default;
  disabled_move& operator=(const disabled_move&) = default;
  disabled_move(disabled_move&&) = delete;
  disabled_move& operator=(disabled_move&&) = delete;
};

struct disabled_copy_move {
  disabled_copy_move() = default;
  disabled_copy_move(const disabled_copy_move&) = delete;
  disabled_copy_move& operator=(const disabled_copy_move&) = delete;
  disabled_copy_move(disabled_copy_move&&) = delete;
  disabled_copy_move& operator=(disabled_copy_move&&) = delete;
};

}
#endif //ESTD_UTILS_H

#ifndef ESTD_UTILS_H
#define ESTD_UTILS_H

namespace estd {

struct disabled_copy {
  disabled_copy() = default;
  disabled_copy(const disabled_copy&) = delete;
  disabled_copy& operator=(const disabled_copy&) = delete;
};

struct disabled_move {
  disabled_move() = default;
  disabled_move(disabled_move&&) = delete;
  disabled_move& operator=(disabled_move&&) = delete;
};

struct disabled_copy_move : disabled_copy, disabled_move {};

}

#endif //ESTD_UTILS_H

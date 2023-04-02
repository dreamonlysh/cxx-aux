#include "binary/elf_parser.h"

namespace binary {

struct demo {
  int x;
  int y;
  int z;
};

void xxx() {
  BinaryBuffer bb("");
  auto x = bb.get<int>();
  auto y = bb.get<double>();
  auto z = bb.get<demo>();


  auto zz = bb.get<demo>();
}

}// namespace binary

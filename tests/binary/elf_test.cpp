#include "binary/elf_parser.h"
#include <fstream>
#include <gtest/gtest.h>

using namespace binary;
using namespace binary::elf;

TEST(BinaryElf, load) {
  std::ifstream fs;
  fs.open("/Users/dreamonly/Downloads/sublime_text/libssl.so.1.1",
          std::ios::out | std::ios::in);
  std::string dynElf{std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>()};

  BinaryBuffer bb(dynElf.c_str(), dynElf.size());
  std::unique_ptr<BinaryElf> be = createBinaryElf(bb);
}
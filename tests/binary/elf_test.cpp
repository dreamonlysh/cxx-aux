#include "binary/elf/elf.h"
#include <fstream>
#include <gtest/gtest.h>

using namespace binary;

TEST(BinaryElf, load) {
  std::ifstream fs;
  fs.open("/Users/dreamonly/Downloads/sublime_text/libssl.so.1.1",
          std::ios::out | std::ios::in);
  std::string dynElf{std::istreambuf_iterator<char>(fs),
                     std::istreambuf_iterator<char>()};

  BinaryBuffer bb(dynElf.c_str(), dynElf.size());
  std::unique_ptr<Elf> be = createBinaryElf(bb);
  size_t size = be->s_size();
  for (size_t i = 0; i < size; ++i) {
    std::unique_ptr<Section> s = be->s_at(i);
    const auto* c = s->dynCast<ExecuteSection>();
    if (c == nullptr) {
    }
    if (s->flags() & SHFlags::SHF_EXECINSTR) {
      auto* a = static_cast<ExecuteSection*>(s.get());
      if (a == nullptr)
        return;
    }
  }
  be->dump();
}
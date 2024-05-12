#include "cxxaux/binary/elf/elf.h"
#include <fstream>
#include <gtest/gtest.h>

using namespace cxxaux::elf;

// TEST(BinaryElf, load) {
//   std::ifstream fs;
//   fs.open("/Users/dreamonly/Downloads/sublime_text/libssl.so.1.1",
//           std::ios::out | std::ios::in);
//   std::string dynElf{std::istreambuf_iterator<char>(fs),
//                      std::istreambuf_iterator<char>()};
//
//   std::unique_ptr<Elf> be = createBinaryElf(dynElf.c_str(), dynElf.size());
//   size_t size = be->s_size();
//   for (size_t i = 0; i < size; ++i) {
//     std::unique_ptr<Section> s = be->s_at(i);
//     if (s->flags() & SHFlags::SHF_EXECINSTR) {
//     }
//   }
//   be->dump();
// }
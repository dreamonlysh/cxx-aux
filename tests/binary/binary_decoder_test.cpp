#include <binary/binary_decoder.h>
#include <gtest/gtest.h>

using cxxaux::BinaryDecoder;

TEST(BinaryBuffer, tellg0seekg) {
  std::string_view data("that is a demo");
  BinaryDecoder buf(data.data(), data.size());

  ASSERT_EQ(buf.tellg(), 0);
  buf.seekg(4);
  ASSERT_EQ(buf.tellg(), 4);
}

TEST(BinaryBuffer, ignore) {
  std::string_view data("that is a demo");
  BinaryDecoder buf(data.data(), data.size());

  buf.seekg(2);
  ASSERT_EQ(buf.ignore("is"), strlen("that "));
  buf.seekg(2);
  ASSERT_EQ(buf.ignore("demo"), strlen("that is a "));
  buf.seekg(2);
  ASSERT_EQ(buf.ignore("are"), BinaryDecoder::eob);
  ASSERT_EQ(buf.tellg(), 2);
}

TEST(BinaryBuffer, get) {
  std::string_view data("\xaa"
                        "\xbb\xbb"
                        "\xcc\xcc\xcc\xcc"
                        "\xee\xee\xee\xee\xee\xee\xee\xee"
                        "\x23\x01"
                        "\x78\x56\x04\x00",
                        21);
  BinaryDecoder buf(data.data(), data.size());

  ASSERT_EQ(buf.get<uint8_t>(), 0xaa);
  ASSERT_EQ(buf.get<uint16_t>(), 0xbbbb);
  ASSERT_EQ(buf.get<uint32_t>(), 0xcccccccc);
  ASSERT_EQ(buf.get<uint64_t>(), 0xeeeeeeeeeeeeeeee);
  struct Tmp {
    uint16_t x;
    uint32_t y;
  } __attribute__((packed, aligned(1)));
  auto* tmp2 = buf.get<Tmp>();
  ASSERT_EQ(tmp2->x, 0x123);
  ASSERT_EQ(tmp2->y, 0x45678);
}

TEST(BinaryBuffer, getn) {
  std::string_view data("that is a demo");
  BinaryDecoder buf(data.data(), data.size());

  buf.seekg(strlen("this "));
  ASSERT_EQ(buf.getn(2), "is");
}

TEST(BinaryBuffer, getc) {
  std::string_view data("that is\0 a demo", 15);
  BinaryDecoder buf(data.data(), data.size());

  buf.seekg(strlen("this "));
  ASSERT_EQ(buf.getc(), "is");
}

TEST(BinaryBuffer, slice) {
  std::string_view data("that is a demo");
  BinaryDecoder buf(data.data(), data.size());

  BinaryDecoder sub = buf.slice(strlen("this "), 2);
  ASSERT_EQ(sub.getn(2), "is");
}

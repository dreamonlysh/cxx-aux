// bit.h example: bit manipulation utilities

#include <bitset>
#include <estd/bit.h>
#include <iostream>

template <typename T>
void print_binary(T v, const char* label) {
  std::cout << label << ": " << std::bitset<sizeof(T) * 8>(v) << " (" << v
            << ")\n";
}

int main() {
  std::cout << "=== bit.h Example ===\n\n";

  std::cout << "1. Counting bits:\n";
  unsigned value = 0b10101010;
  print_binary(value, "value");
  std::cout << "count_bit1: " << es::count_bit1(value) << " bits set\n";
  std::cout << "count_bit0: " << es::count_bit0(value) << " bits clear\n";
  std::cout << "countl_bit0: " << es::countl_bit0(value) << " leading zeros\n";
  std::cout << "countr_bit0: " << es::countr_bit0(value)
            << " trailing zeros\n\n";

  std::cout << "2. Highlight and dim operations:\n";
  value = 0b10100111;
  print_binary(value, "value");
  print_binary(es::hl_first_bit0(value), "hl_first_bit0");
  print_binary(es::hl_first_bit1(value), "hl_first_bit1");
  print_binary(es::dim_first_bit1(value), "dim_first_bit1");
  std::cout << "\n";

  value = 0b01011000;
  print_binary(value, "value");
  print_binary(es::hlr_bit0(value), "hlr_bit0 (highlight trailing zeros)");
  print_binary(es::hlr_bit0_inc(value), "hlr_bit0_inc (include first 1)");
  std::cout << "\n";

  value = 0b10100111;
  print_binary(value, "value");
  print_binary(es::dimr_bit1(value), "dimr_bit1 (dim trailing ones)");
  print_binary(es::hlr_bit1_inc(value), "hlr_bit1_inc (include first 0)");
  std::cout << "\n";

  std::cout << "3. Power of 2 operations:\n";
  std::cout << "is_pow2(8u): " << std::boolalpha << es::is_pow2(8u) << "\n";
  std::cout << "is_pow2(6u): " << es::is_pow2(6u) << "\n";
  std::cout << "is_pow2_sub1(7u): " << es::is_pow2_sub1(7u)
            << " (7 = 2^3 - 1)\n";
  std::cout << "is_pow2_sub1(6u): " << es::is_pow2_sub1(6u) << "\n";
  std::cout << "is_pow2_sub_pow2(12u): " << es::is_pow2_sub_pow2(12u)
            << " (12 = 2^4 - 2^2)\n";
  std::cout << "is_pow2_sub_pow2(10u): " << es::is_pow2_sub_pow2(10u) << "\n\n";

  std::cout << "4. Rounding to power of 2:\n";
  std::cout << "floor_to_pow2(100u): " << es::floor_to_pow2(100u) << "\n";
  std::cout << "ceil_to_pow2(100u): " << es::ceil_to_pow2(100u) << "\n";
  std::cout << "floor_to_pow2(1023u): " << es::floor_to_pow2(1023u) << "\n";
  std::cout << "ceil_to_pow2(1023u): " << es::ceil_to_pow2(1023u) << "\n\n";

  std::cout << "5. Get and set bits:\n";
  value = 0b11010110;
  print_binary(value, "value");
  std::cout << "get_bit(value, 0): " << es::get_bit(value, 0) << "\n";
  std::cout << "get_bit(value, 1): " << es::get_bit(value, 1) << "\n";
  std::cout << "get_bits(value, 2, 3): " << es::get_bits(value, 2, 3)
            << " (bits 2-4)\n";
  std::cout << "get_bits(value, 4, 4): " << es::get_bits(value, 4, 4)
            << " (bits 4-7)\n\n";

  std::cout << "6. Set and reset operations:\n";
  value = 0u;
  print_binary(es::set_bit(value, 3), "set_bit(0, 3)");
  print_binary(es::set_bits(value, 2, 4), "set_bits(0, 2, 4)");

  value = 0b01011000;
  print_binary(value, "value");
  print_binary(es::reset_first(value), "reset_first (clear rightmost 1)");
  print_binary(es::set_first(value), "set_first (set rightmost 0)");
  print_binary(es::resetr(value), "resetr (clear trailing 1s)");
  print_binary(es::setr(value), "setr (set trailing 0s)");
  std::cout << "\n";

  std::cout << "7. Reset adjacent bits:\n";
  value = 0b01011100;
  print_binary(value, "value");
  print_binary(es::reset_first_adjacent(value), "reset_first_adjacent");
  std::cout << "\n";

  std::cout << "8. Practical example: iterating through set bits:\n";
  value = 0b10101010;
  print_binary(value, "value");
  std::cout << "Set bit positions: ";
  while (value != 0) {
    unsigned pos = es::countr_bit0(es::hl_first_bit1(value));
    std::cout << pos << " ";
    value = es::reset_first(value);
  }
  std::cout << "\n\n";

  std::cout << "9. Practical example: finding contiguous bit ranges:\n";
  value = 0b01110000;
  print_binary(value, "value");
  unsigned start = es::countr_bit0(value);
  unsigned length = es::count_bit1(es::hlr_bit0_inc(es::resetr(value)));
  std::cout << "Contiguous 1s: start=" << start << ", length=" << length
            << "\n";

  return 0;
}

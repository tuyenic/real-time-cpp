///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2016.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CRC32_2015_01_26_H_
  #define CRC32_2015_01_26_H_

  #include <array>
  #include <cstdint>
  #include <numeric>

  namespace math
  {
    namespace checksums
    {
      template<typename input_iterator>
      std::uint32_t crc32_mpeg2(input_iterator first, input_iterator last)
      {
        // Name            : CRC32/MPEG-2
        // Polynomial      : 0x04C1'1DB7
        // Initial value   : 0xFFFF'FFFF
        // Test: '1'...'9' : 0x0376'E6E7

        // See also ISO/IEC 13818-1:2000, Recommendation H.222.0 Annex A

        // Define a local_value_type.
        typedef typename std::iterator_traits<input_iterator>::value_type local_value_type;

        // Set the initial value. Also loop through
        // the input data stream using accumulate
        // in combination with a lambda function.
        return std::accumulate(
          first,
          last,
          UINT32_C(0xFFFFFFFF),
          [](std::uint32_t crc, const local_value_type& data) -> std::uint32_t
          {
            // CRC32/MPEG2 Table based on nibbles.
            const std::array<std::uint32_t, 16U> table =
            {{
              UINT32_C(0x00000000), UINT32_C(0x04C11DB7),
              UINT32_C(0x09823B6E), UINT32_C(0x0D4326D9),
              UINT32_C(0x130476DC), UINT32_C(0x17C56B6B),
              UINT32_C(0x1A864DB2), UINT32_C(0x1E475005),
              UINT32_C(0x2608EDB8), UINT32_C(0x22C9F00F),
              UINT32_C(0x2F8AD6D6), UINT32_C(0x2B4BCB61),
              UINT32_C(0x350C9B64), UINT32_C(0x31CD86D3),
              UINT32_C(0x3C8EA00A), UINT32_C(0x384FBDBD)
            }};

            const std::uint_fast8_t the_byte = uint_fast8_t(data & UINT8_C(0xFF));

            std::uint_fast8_t index;

            // Perform the CRC32/MPEG2 algorithm.
            index = ((std::uint_fast8_t(crc >> 28)) ^ (std::uint_fast8_t(the_byte >>  4))) & UINT8_C(0x0F);

            crc   = std::uint32_t(std::uint32_t(crc << 4) & UINT32_C(0xFFFFFFF0)) ^ table[index];

            index = ((std::uint_fast8_t(crc >> 28)) ^ (std::uint_fast8_t(the_byte))) & UINT8_C(0x0F);

            crc   = std::uint32_t(std::uint32_t(crc << 4) & UINT32_C(0xFFFFFFF0)) ^ table[index];

            return crc;
          });
      }
    }
  }

  /*
  const std::array<std::uint8_t, 9U> data =
  {{
    0x31U, 0x32U, 0x33U, 0x34U, 0x35U, 0x36U, 0x37U, 0x38U, 0x39U
  }};

  // The crc32 reference value is 0x0376E6E7.
  const std::uint32_t crc = math::checksums::crc32_mpeg2(data.cbegin(), data.cend());
  */

#endif // CRC32_2015_01_26_H_

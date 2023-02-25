// Catch2
#include <catch_amalgamated.hpp>

// BARE
#include <bare/bare.hpp>

namespace bv = bare::value;

// https://www.ietf.org/archive/id/draft-devault-bare-08.html#appendix-A
TEST_CASE("encoding RFC example values", "[encoding]") {
  bv::byte_buffer_t buffer;

  SECTION("uint") {
    auto [input_value, output_bytes] = GENERATE(table<uint64_t, bv::byte_buffer_t>({
      {0,   bv::byte_buffer_t{ std::byte{0x00}                  }},
      {1,   bv::byte_buffer_t{ std::byte{0x01}                  }},
      {126, bv::byte_buffer_t{ std::byte{0x7e}                  }},
      {127, bv::byte_buffer_t{ std::byte{0x7f}                  }},
      {128, bv::byte_buffer_t{ std::byte{0x80}, std::byte{0x01} }},
      {129, bv::byte_buffer_t{ std::byte{0x81}, std::byte{0x01} }},
      {255, bv::byte_buffer_t{ std::byte{0xFF}, std::byte{0x01} }}
    }));

    CAPTURE(input_value);

    auto value = bv::UintValue(input_value);
    value.encode(buffer);

    REQUIRE(buffer.size() == output_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), output_bytes.begin()));
  }

  SECTION("int") {
    auto [input_value, output_bytes] = GENERATE(table<int64_t, bv::byte_buffer_t>({
      { 0,   bv::byte_buffer_t{ std::byte{0x00}                  }},
      { 1,   bv::byte_buffer_t{ std::byte{0x02}                  }},
      {-1,   bv::byte_buffer_t{ std::byte{0x01}                  }},
      { 63,  bv::byte_buffer_t{ std::byte{0x7E}                  }},
      {-63,  bv::byte_buffer_t{ std::byte{0x7d}                  }},
      { 64,  bv::byte_buffer_t{ std::byte{0x80}, std::byte{0x01} }},
      {-64,  bv::byte_buffer_t{ std::byte{0x7F}                  }},
      { 65,  bv::byte_buffer_t{ std::byte{0x82}, std::byte{0x01} }},
      {-65,  bv::byte_buffer_t{ std::byte{0x81}, std::byte{0x01} }},
      { 255, bv::byte_buffer_t{ std::byte{0xFE}, std::byte{0x03} }},
      {-255, bv::byte_buffer_t{ std::byte{0xFD}, std::byte{0x03} }}
    }));

    CAPTURE(input_value);

    auto value = bv::IntValue(input_value);
    value.encode(buffer);

    REQUIRE(buffer.size() == output_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), output_bytes.begin()));
  }

  SECTION("u32") {
    auto [input_value, output_bytes] = GENERATE(table<uint32_t, bv::byte_buffer_t>({
      {0,   bv::byte_buffer_t{ std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00} }},
      {1,   bv::byte_buffer_t{ std::byte{0x01}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00} }},
      {255, bv::byte_buffer_t{ std::byte{0xFF}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00} }}
    }));

    CAPTURE(input_value);

    auto value = bv::Uint32Value(input_value);
    value.encode(buffer);

    REQUIRE(buffer.size() == output_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), output_bytes.begin()));
  }

  SECTION("i16") {
    auto [input_value, output_bytes] = GENERATE(table<int16_t, bv::byte_buffer_t>({
      { 0,   bv::byte_buffer_t{ std::byte{0x00}, std::byte{0x00} }},
      { 1,   bv::byte_buffer_t{ std::byte{0x01}, std::byte{0x00} }},
      {-1,   bv::byte_buffer_t{ std::byte{0xFF}, std::byte{0xFF} }},
      { 255, bv::byte_buffer_t{ std::byte{0xFF}, std::byte{0x00} }},
      {-255, bv::byte_buffer_t{ std::byte{0x01}, std::byte{0xFF} }}
    }));

    CAPTURE(input_value);

    auto value = bv::Int16Value(input_value);
    value.encode(buffer);

    REQUIRE(buffer.size() == output_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), output_bytes.begin()));
  }

  SECTION("f64") {
    auto [input_value, output_bytes] = GENERATE(table<double, bv::byte_buffer_t>({
      { 0.0,  bv::byte_buffer_t{
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00}  }},
      { 1.0,  bv::byte_buffer_t{
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x00}, std::byte{0xF0}, std::byte{0x3F}  }},
      { 2.55, bv::byte_buffer_t{
        std::byte{0x66}, std::byte{0x66}, std::byte{0x66}, std::byte{0x66},
        std::byte{0x66}, std::byte{0x66}, std::byte{0x04}, std::byte{0x40}  }},
      {-25.5, bv::byte_buffer_t{
        std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
        std::byte{0x00}, std::byte{0x80}, std::byte{0x39}, std::byte{0xC0}  }},
    }));

    CAPTURE(input_value);

    auto value = bv::Float64Value(input_value);
    value.encode(buffer);

    REQUIRE(buffer.size() == output_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), output_bytes.begin()));
  }

  SECTION("bool") {
    auto [input_value, output_bytes] = GENERATE(table<bool, bv::byte_buffer_t>({
      {true,  bv::byte_buffer_t{ std::byte{0x01} }},
      {false, bv::byte_buffer_t{ std::byte{0x00} }}
    }));

    CAPTURE(input_value);

    auto value = bv::BoolValue(input_value);
    value.encode(buffer);

    REQUIRE(buffer.size() == output_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), output_bytes.begin()));
  }

  // TODO: str
  // TODO: data
  // TODO: data[16]
  // TODO: void
  // TODO: enum
  // TODO: optional<u32>
  // TODO: list<str>
  // TODO: list<uint>[10]
  // TODO: map<u32><str>
  // TODO: union
  // TODO: struct
}

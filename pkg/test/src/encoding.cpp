// Catch2
#include <catch_amalgamated.hpp>

// BARE
#include <bare/bare.hpp>

namespace bv = bare::value;

// https://www.ietf.org/archive/id/draft-devault-bare-08.html#appendix-A
TEST_CASE("RFC example values", "[rfc]") {
  bv::byte_buffer_t buffer;

  SECTION("uint") {
    auto [naked_value, encoded_bytes] = GENERATE(table<uint64_t, bv::byte_buffer_t>({
      {0,   bv::byte_buffer_t{ std::byte{0x00}                  }},
      {1,   bv::byte_buffer_t{ std::byte{0x01}                  }},
      {126, bv::byte_buffer_t{ std::byte{0x7e}                  }},
      {127, bv::byte_buffer_t{ std::byte{0x7f}                  }},
      {128, bv::byte_buffer_t{ std::byte{0x80}, std::byte{0x01} }},
      {129, bv::byte_buffer_t{ std::byte{0x81}, std::byte{0x01} }},
      {255, bv::byte_buffer_t{ std::byte{0xFF}, std::byte{0x01} }}
    }));

    CAPTURE(naked_value);
    CAPTURE(encoded_bytes);

    // encoding
    auto value = bv::UintValue(naked_value);
    value.encode(buffer);

    CAPTURE(buffer);
    REQUIRE(buffer.size() == encoded_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), encoded_bytes.begin()));

    // decoding
    auto [decoded_value, decoded_span] = bv::UintValue::decode(buffer);

    CAPTURE(decoded_value);
    CAPTURE(decoded_span);
    REQUIRE(decoded_span.size() == buffer.size());
    REQUIRE(decoded_value == naked_value);
  }

  SECTION("int") {
    auto [naked_value, encoded_bytes] = GENERATE(table<int64_t, bv::byte_buffer_t>({
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

    CAPTURE(naked_value);
    CAPTURE(encoded_bytes);

    // encoding
    auto value = bv::IntValue(naked_value);
    value.encode(buffer);

    CAPTURE(buffer);
    REQUIRE(buffer.size() == encoded_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), encoded_bytes.begin()));

    // decoding
    auto [decoded_value, decoded_span] = bv::IntValue::decode(buffer);

    CAPTURE(decoded_value);
    CAPTURE(decoded_span);
    REQUIRE(decoded_span.size() == buffer.size());
    REQUIRE(decoded_value == naked_value);
  }

  SECTION("u32") {
    auto [naked_value, encoded_bytes] = GENERATE(table<uint32_t, bv::byte_buffer_t>({
      {0,   bv::byte_buffer_t{ std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00} }},
      {1,   bv::byte_buffer_t{ std::byte{0x01}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00} }},
      {255, bv::byte_buffer_t{ std::byte{0xFF}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00} }}
    }));

    CAPTURE(naked_value);
    CAPTURE(encoded_bytes);

    // encoding
    auto value = bv::Uint32Value(naked_value);
    value.encode(buffer);

    CAPTURE(buffer);
    REQUIRE(buffer.size() == encoded_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), encoded_bytes.begin()));

    // decoding
    auto [decoded_value, decoded_span] = bv::Uint32Value::decode(buffer);

    CAPTURE(decoded_value);
    CAPTURE(decoded_span);
    REQUIRE(decoded_span.size() == buffer.size());
    REQUIRE(decoded_value == naked_value);
  }

  SECTION("i16") {
    auto [naked_value, encoded_bytes] = GENERATE(table<int16_t, bv::byte_buffer_t>({
      { 0,   bv::byte_buffer_t{ std::byte{0x00}, std::byte{0x00} }},
      { 1,   bv::byte_buffer_t{ std::byte{0x01}, std::byte{0x00} }},
      {-1,   bv::byte_buffer_t{ std::byte{0xFF}, std::byte{0xFF} }},
      { 255, bv::byte_buffer_t{ std::byte{0xFF}, std::byte{0x00} }},
      {-255, bv::byte_buffer_t{ std::byte{0x01}, std::byte{0xFF} }}
    }));

    CAPTURE(naked_value);
    CAPTURE(encoded_bytes);

    // encoding
    auto value = bv::Int16Value(naked_value);
    value.encode(buffer);

    CAPTURE(buffer);
    REQUIRE(buffer.size() == encoded_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), encoded_bytes.begin()));

    // decoding
    auto [decoded_value, decoded_span] = bv::Int16Value::decode(buffer);

    CAPTURE(decoded_value);
    CAPTURE(decoded_span);
    REQUIRE(decoded_span.size() == buffer.size());
    REQUIRE(decoded_value == naked_value);
  }

  SECTION("f64") {
    auto [naked_value, encoded_bytes] = GENERATE(table<double, bv::byte_buffer_t>({
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

    CAPTURE(naked_value);
    CAPTURE(encoded_bytes);

    // encoding
    auto value = bv::Float64Value(naked_value);
    value.encode(buffer);

    CAPTURE(buffer);
    REQUIRE(buffer.size() == encoded_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), encoded_bytes.begin()));

    // decoding
    auto [decoded_value, decoded_span] = bv::Float64Value::decode(buffer);

    CAPTURE(decoded_value);
    CAPTURE(decoded_span);
    REQUIRE(decoded_span.size() == buffer.size());
    REQUIRE(decoded_value == naked_value);
  }

  SECTION("bool") {
    auto [naked_value, encoded_bytes] = GENERATE(table<bool, bv::byte_buffer_t>({
      {true,  bv::byte_buffer_t{ std::byte{0x01} }},
      {false, bv::byte_buffer_t{ std::byte{0x00} }}
    }));

    CAPTURE(naked_value);
    CAPTURE(encoded_bytes);

    // encoding
    auto value = bv::BoolValue(naked_value);
    value.encode(buffer);

    CAPTURE(buffer);
    REQUIRE(buffer.size() == encoded_bytes.size());
    REQUIRE(std::equal(buffer.begin(), buffer.end(), encoded_bytes.begin()));

    // decoding
    auto [decoded_value, decoded_span] = bv::BoolValue::decode(buffer);

    CAPTURE(decoded_value);
    CAPTURE(decoded_span);
    REQUIRE(decoded_span.size() == buffer.size());
    REQUIRE(decoded_value == naked_value);
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

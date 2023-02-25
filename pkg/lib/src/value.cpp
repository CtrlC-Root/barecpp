#include <bare/value.hpp>

// C++ Standard Library
#include <algorithm>
#include <array>
#include <bit>

#include <iostream> // DEBUG

using namespace bare::value;


/**
 * UintValue
 */

UintValue::UintValue()
: value(0) {
  // nothing to do
}

UintValue::UintValue(const uint64_t& initial)
: value(initial) {
  // nothing to do
}

UintValue::operator uint64_t() const {
  return this->value;
}

UintValue& UintValue::operator =(const uint64_t& value) {
  this->value = value;
  return *this;
}

void UintValue::encode(byte_buffer_t& buffer) const {
  uint64_t value = this->value;

  // little endian encoding
  static_assert(std::endian::native == std::endian::big || std::endian::native == std::endian::little);
  if (std::endian::native == std::endian::big) {
    auto data = std::bit_cast<std::array<std::byte, sizeof(uint64_t)>>(value);
    std::ranges::reverse(data);
    value = std::bit_cast<uint64_t>(data);
  }

  // https://en.wikipedia.org/wiki/LEB128#Unsigned_LEB128
  do {
    auto current_byte = std::byte(0b01111111 & value);
    value >>= 7;

    if (value != 0) {
      current_byte |= std::byte{0b10000000};
    }

    buffer.push_back(current_byte);
  } while (value != 0);
}

#include <bitset>

std::ostream& operator<< (std::ostream& os, std::byte b) {
    return os << std::bitset<8>(std::to_integer<int>(b));
}

std::pair<UintValue, byte_span_t> UintValue::decode(const byte_span_t& source) {
  // https://en.wikipedia.org/wiki/LEB128#Decode_unsigned_integer
  // TODO: https://en.wikipedia.org/wiki/LEB128#Fast_decoding

  byte_span_t relevant;
  for (size_t i = 0; i < source.size(); i++) {
    if ((source[i] & std::byte{0b10000000}) == std::byte{0}) {
      relevant = source.subspan(0, i + 1);
      break;
    }
  }

  if (relevant.size() == 0) {
    throw std::runtime_error("insufficient bytes in source");
  }

  // little endian encoding
  uint64_t value = 0;
  static_assert(std::endian::native == std::endian::big || std::endian::native == std::endian::little);
  if (std::endian::native == std::endian::big) {
    for (auto it = relevant.begin(); it != relevant.end(); ++it) {
      value <<= 7;
      value |= std::to_integer<uint8_t>(*it & std::byte{0b01111111});
    }
  } else {
    for (auto it = relevant.rbegin(); it != relevant.rend(); ++it) {
      value <<= 7;
      value |= std::to_integer<uint8_t>(*it & std::byte{0b01111111});
    }
  }

  return {UintValue(value), relevant};
}

/**
 * IntValue
 */

IntValue::IntValue()
: value(0) {
  // nothing to do
}

IntValue::IntValue(const int64_t& initial)
: value(initial) {
  // nothing to do
}

IntValue::operator int64_t() const {
  return this->value;
}

IntValue& IntValue::operator =(const int64_t& value) {
  this->value = value;
  return *this;
}

void IntValue::encode(byte_buffer_t& buffer) const {
  if (this->value >= 0) {
    auto positive_value = UintValue(2 * this->value);
    positive_value.encode(buffer);
  } else {
    auto negative_value = UintValue((-2 * this->value) - 1);
    negative_value.encode(buffer);
  }
}

std::pair<IntValue, byte_span_t> IntValue::decode(const byte_span_t& source) {
  // https://en.wikipedia.org/wiki/LEB128#Decode_unsigned_integer
  // TODO: https://en.wikipedia.org/wiki/LEB128#Fast_decoding

  auto [decoded_uint, decoded_span] = UintValue::decode(source);
  if (decoded_uint % 2 == 0) {
    int64_t value = static_cast<int64_t>(decoded_uint / 2);
    return {IntValue(value), decoded_span};
  } else {
    int64_t value = -1 * static_cast<int64_t>((decoded_uint + 1) / 2);
    return {IntValue(value), decoded_span};
  }
}

/**
 * FixedIntValue
 */

template <typename T, typename V>
FixedIntValue<T, V>::FixedIntValue()
: value(0) {
  // nothing to do
}

template <typename T, typename V>
FixedIntValue<T, V>::FixedIntValue(const T& initial)
: value(initial) {
  // nothing to do
}

template <typename T, typename V>
FixedIntValue<T, V>::operator T() const {
  return this->value;
}

template <typename T, typename V>
FixedIntValue<T, V>& FixedIntValue<T, V>::operator =(const T& value) {
  this->value = value;
  return *this;
}

template <typename T, typename V>
void FixedIntValue<T, V>::encode(byte_buffer_t& buffer) const {
  // bitwise equivalent representation as an array of bytes
  const auto data = std::bit_cast<std::array<std::byte, sizeof(T)>>(this->value);

  // little endian encoding
  static_assert(std::endian::native == std::endian::big || std::endian::native == std::endian::little);
  if (std::endian::native == std::endian::little) {
    buffer.insert(buffer.end(), data.begin(), data.end());
  } else {
    buffer.insert(buffer.end(), data.rbegin(), data.rend());
  }
}

template <typename T, typename V>
std::pair<V, byte_span_t> FixedIntValue<T, V>::decode(const byte_span_t& source) {
  if (sizeof(T) > source.size()) {
    throw std::runtime_error("insufficient bytes in source");
  }

  byte_span_t relevant = source.subspan(0, sizeof(T));
  std::array<std::byte, sizeof(T)> data;
  std::copy(relevant.begin(), relevant.end(), data.begin());

  // little endian encoding
  static_assert(std::endian::native == std::endian::big || std::endian::native == std::endian::little);
  if (std::endian::native == std::endian::big) {
    std::ranges::reverse(data);
  }

  T value = std::bit_cast<T>(data);
  return {V(value), relevant};
}

template class FixedIntValue<uint8_t, Uint8Value>;
template class FixedIntValue<uint16_t, Uint16Value>;
template class FixedIntValue<uint32_t, Uint32Value>;
template class FixedIntValue<uint64_t, Uint64Value>;
template class FixedIntValue<int8_t, Int8Value>;
template class FixedIntValue<int16_t, Int16Value>;
template class FixedIntValue<int32_t, Int32Value>;
template class FixedIntValue<int64_t, Int64Value>;

/**
 * FixedFloatValue
 */

template <typename T, typename V>
FixedFloatValue<T, V>::FixedFloatValue()
: value(0.0) {
  // nothing to do
}

template <typename T, typename V>
FixedFloatValue<T, V>::FixedFloatValue(const T& initial)
: value(initial) {
  // nothing to do
}

template <typename T, typename V>
FixedFloatValue<T, V>::operator T() const {
  return this->value;
}

template <typename T, typename V>
FixedFloatValue<T, V>& FixedFloatValue<T, V>::operator =(const T& value) {
  this->value = value;
  return *this;
}

template <typename T, typename V>
void FixedFloatValue<T, V>::encode(byte_buffer_t& buffer) const {
  // TODO: static assert for IEEE 754 float and double representation
  // https://kkimdev.github.io/posts/2018/06/15/IEEE-754-Floating-Point-Type-in-C++.html

  // bitwise equivalent representation as an array of bytes
  const auto data = std::bit_cast<std::array<std::byte, sizeof(T)>>(this->value);
  buffer.insert(buffer.end(), data.begin(), data.end());
}

template <typename T, typename V>
std::pair<V, byte_span_t> FixedFloatValue<T, V>::decode(const byte_span_t& source) {
  if (sizeof(T) > source.size()) {
    throw std::runtime_error("insufficient bytes in source");
  }

  // TODO: static assert for IEEE 754 float and double representation
  // https://kkimdev.github.io/posts/2018/06/15/IEEE-754-Floating-Point-Type-in-C++.html

  byte_span_t relevant = source.subspan(0, sizeof(T));
  std::array<std::byte, sizeof(T)> data;
  std::copy(relevant.begin(), relevant.end(), data.begin());

  T value = std::bit_cast<T>(data);
  return {V(value), relevant};
}

template class FixedFloatValue<float, Float32Value>;
template class FixedFloatValue<double, Float64Value>;

/**
 * BoolValue
 */

BoolValue::BoolValue()
:value(false) {
  // nothing to do
}

BoolValue::BoolValue(const bool& initial)
: value(initial) {
  // nothing to do
}

BoolValue::operator bool() const {
  return this->value;
}

BoolValue& BoolValue::operator =(const bool& value) {
  this->value = value;
  return *this;
}

void BoolValue::encode(byte_buffer_t& buffer) const {
  if (this->value) {
    buffer.push_back(std::byte{1});
  } else {
    buffer.push_back(std::byte{0});
  }
}

std::pair<BoolValue, byte_span_t> BoolValue::decode(const byte_span_t& source) {
  if (source.size() == 0) {
    throw std::runtime_error("insufficient bytes in source");
  }

  byte_span_t relevant = source.subspan(0, 1);
  if (relevant[0] == std::byte{0}) {
    return {BoolValue(false), relevant};
  } else {
    return {BoolValue(true), relevant};
  }
}

/**
 * StrValue
 */

void StrValue::encode(byte_buffer_t& buffer) const {
  // string length as uint value
  auto uint_length = UintValue(this->value.size());
  uint_length.encode(buffer);

  // string bytes
  std::transform(
    std::begin(this->value),
    std::end(this->value),
    std::back_inserter(buffer),
    [](char c) {
      return std::byte(c);
    });
}

/**
 * DataValue
 */

void DataValue::encode(byte_buffer_t& buffer) const {
  // variable length data values encode their length as a uint value first
  if (!this->fixed_size.has_value()) {
    auto data_size = UintValue(this->value.size());
    data_size.encode(buffer);
  }

  buffer.insert(buffer.end(), this->value.begin(), this->value.end());
}

/**
 * VoidValue
 */

void VoidValue::encode(byte_buffer_t& buffer) const {
  // nothing to do
}

/**
 * OptionalValue
 */

void OptionalValue::encode(byte_buffer_t& buffer) const {
  if (auto value = this->value.get(); value == nullptr) {
    buffer.push_back(std::byte{0});
    return;
  }
  
  buffer.push_back(std::byte{1});
  std::visit(
    [&buffer](const auto& item) { item.encode(buffer); },
    *value);
}

/**
 * ListValue
 */

void ListValue::encode(byte_buffer_t& buffer) const {
  // variable length list values encode their length as uint value first
  if (!this->fixed_size.has_value()) {
    auto list_size = UintValue(this->values.size());
    list_size.encode(buffer);
  }

  // TODO: validate items all have the same type
  for (auto const &value : this->values) {
    std::visit(
      [&buffer](const auto& item) { item.encode(buffer); },
      value);
  }
}

/**
 * MapValue
 */

void MapValue::encode(byte_buffer_t& buffer) const {
  // number of map pairs as uint value
  auto pair_count = UintValue(this->values.size());
  pair_count.encode(buffer);

  // encode (key, value) pairs sequentially
  for (const auto& [key, value] : this->values) {
    std::visit(
      [&buffer](const auto& item) { item.encode(buffer); },
      key);

    std::visit(
      [&buffer](const auto& item) { item.encode(buffer); },
      value);
  }
}

/**
 * UnionValue
 */

void UnionValue::encode(byte_buffer_t& buffer) const {
  auto value = this->value.get();
  if (value == nullptr) {
    throw std::logic_error("uh oh");
  }

  this->tag.encode(buffer);
  std::visit(
    [&buffer](const auto& item) { item.encode(buffer); },
    *value);
}

/**
 * StructValue
 */

void StructValue::encode(byte_buffer_t& buffer) const {
  // TODO: must have at least 1 field
  for (const auto& entry : this->fields) {
    std::visit(
      [&buffer](const auto& item) { item.encode(buffer); },
      entry.second);
  }
}

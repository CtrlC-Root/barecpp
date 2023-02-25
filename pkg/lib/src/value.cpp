#include <bare/value.hpp>

// C++ Standard Library
#include <algorithm>
#include <array>
#include <bit>

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

/**
 * FixedIntValue
 */

template <typename T>
FixedIntValue<T>::FixedIntValue()
: value(0) {
  // nothing to do
}

template <typename T>
FixedIntValue<T>::FixedIntValue(const T& initial)
: value(initial) {
  // nothing to do
}

template <typename T>
FixedIntValue<T>::operator T() const {
  return this->value;
}

template <typename T>
FixedIntValue<T>& FixedIntValue<T>::operator =(const T& value) {
  this->value = value;
  return *this;
}

template <typename T>
void FixedIntValue<T>::encode(byte_buffer_t& buffer) const {
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

template class FixedIntValue<uint8_t>;
template class FixedIntValue<uint16_t>;
template class FixedIntValue<uint32_t>;
template class FixedIntValue<uint64_t>;
template class FixedIntValue<int8_t>;
template class FixedIntValue<int16_t>;
template class FixedIntValue<int32_t>;
template class FixedIntValue<int64_t>;

/**
 * FixedFloatValue
 */

template <typename T>
FixedFloatValue<T>::FixedFloatValue()
: value(0.0) {
  // nothing to do
}

template <typename T>
FixedFloatValue<T>::FixedFloatValue(const T& initial)
: value(initial) {
  // nothing to do
}

template <typename T>
FixedFloatValue<T>::operator T() const {
  return this->value;
}

template <typename T>
FixedFloatValue<T>& FixedFloatValue<T>::operator =(const T& value) {
  this->value = value;
  return *this;
}

template <typename T>
void FixedFloatValue<T>::encode(byte_buffer_t& buffer) const {
  // TODO: static assert for IEEE 754 float and double representation
  // https://kkimdev.github.io/posts/2018/06/15/IEEE-754-Floating-Point-Type-in-C++.html

  // bitwise equivalent representation as an array of bytes
  const auto data = std::bit_cast<std::array<std::byte, sizeof(T)>>(this->value);
  buffer.insert(buffer.end(), data.begin(), data.end());
}

template class FixedFloatValue<float>;
template class FixedFloatValue<double>;

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

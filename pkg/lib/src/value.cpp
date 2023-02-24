#include <bare/value.hpp>

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
  // TODO
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
  // TODO
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
  // TODO
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
  // TODO
}

template class FixedFloatValue<float>;
template class FixedFloatValue<double>;

/**
 * BoolValue
 */

void BoolValue::encode(byte_buffer_t& buffer) const {
  // TODO
}

/**
 * StrValue
 */

void StrValue::encode(byte_buffer_t& buffer) const {
  // TODO
}

/**
 * DataValue
 */

void DataValue::encode(byte_buffer_t& buffer) const {
  // TODO
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
  // TODO
}

/**
 * ListValue
 */

void ListValue::encode(byte_buffer_t& buffer) const {
  // TODO
}

/**
 * MapValue
 */

void MapValue::encode(byte_buffer_t& buffer) const {
  // TODO
}

/**
 * UnionValue
 */

void UnionValue::encode(byte_buffer_t& buffer) const {
  // TODO
}

/**
 * StructValue
 */

void StructValue::encode(byte_buffer_t& buffer) const {
  // TODO
}

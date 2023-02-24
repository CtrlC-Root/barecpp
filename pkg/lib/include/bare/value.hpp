#pragma once

// C++ Standard Library
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace bare {
  namespace value {
    /**
     * Forward declare value classes.
     */
    class UintValue;
    class Uint8Value;
    class Uint16Value;
    class Uint32Value;
    class Uint64Value;
    class IntValue;
    class Int8Value;
    class Int16Value;
    class Int32Value;
    class Int64Value;
    class Float32Value;
    class Float64Value;
    class BoolValue;
    class StrValue;
    class DataValue;
    class VoidValue;
    class EnumValue;
    class OptionalValue;
    class ListValue;
    class MapValue;
    class UnionValue;
    class StructValue;

    /**
     * User type variant.
     */
    typedef std::variant<
      UintValue,
      Uint8Value,
      Uint16Value,
      Uint32Value,
      Uint64Value,
      IntValue,
      Int8Value,
      Int16Value,
      Int32Value,
      Int64Value,
      Float32Value,
      Float64Value,
      BoolValue,
      StrValue,
      DataValue,
      VoidValue,
      EnumValue,
      OptionalValue,
      ListValue,
      MapValue,
      UnionValue,
      StructValue
    > user_value_t;

    /**
     * TODO.
     */
    typedef std::vector<std::byte> byte_buffer_t;

    /**
     * Abstract base class for values.
     */
    class Value {
    public:
        virtual void encode(byte_buffer_t& buffer) const = 0;
    };

    /**
     * Variable length unsigned integer.
     */
    class UintValue: public Value {
    public:
      // TODO: optionally use https://gmplib.org/ to store arbitrarily large integers
      uint64_t value;

    public:
      UintValue();
      UintValue(const uint64_t& initial);

      operator uint64_t() const;
      UintValue& operator =(const uint64_t& value);

      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Variable length signed integer.
     */
    class IntValue: public Value {
    private:
      // TODO: optionally use https://gmplib.org/ to store arbitrarily large integers
      int64_t value;

    public:
      IntValue();
      IntValue(const int64_t& initial);

      operator int64_t() const;
      IntValue& operator =(const int64_t& value);

      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Abstract base class for fixed integer values.
     */
    template <typename T>
    class FixedIntValue: public Value {
    public:
      T value;

    public:
      FixedIntValue();
      FixedIntValue(const T& initial);

      operator T() const;
      FixedIntValue& operator =(const T& value);

      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Fixed 8-bit unsigned integer.
     */
    class Uint8Value: public FixedIntValue<uint8_t> {
      using FixedIntValue<uint8_t>::FixedIntValue;
    };

    /**
     * Fixed 16-bit unsigned integer.
     */
    class Uint16Value: public FixedIntValue<uint16_t> {
      using FixedIntValue<uint16_t>::FixedIntValue;
    };

    /**
     * Fixed 32-bit unsigned integer.
     */
    class Uint32Value: public FixedIntValue<uint32_t> {
      using FixedIntValue<uint32_t>::FixedIntValue;
    };

    /**
     * Fixed 64-bit unsigned integer.
     */
    class Uint64Value: public FixedIntValue<uint64_t> {
      using FixedIntValue<uint64_t>::FixedIntValue;
    };

    /**
     * Fixed 8-bit signed integer.
     */
    class Int8Value: public FixedIntValue<int8_t> {
      using FixedIntValue<int8_t>::FixedIntValue;
    };

    /**
     * Fixed 16-bit signed integer.
     */
    class Int16Value: public FixedIntValue<int16_t> {
      using FixedIntValue<int16_t>::FixedIntValue;
    };

    /**
     * Fixed 32-bit signed integer.
     */
    class Int32Value: public FixedIntValue<int32_t> {
      using FixedIntValue<int32_t>::FixedIntValue;
    };

    /**
     * Fixed 64-bit signed integer.
     */
    class Int64Value: FixedIntValue<int64_t> {
      using FixedIntValue<int64_t>::FixedIntValue;
    };

    /**
     * Abstract base class for fixed floating point values.
     */
    template <typename T>
    class FixedFloatValue: public Value {
    public:
      T value;

    public:
      FixedFloatValue();
      FixedFloatValue(const T& initial);

      operator T() const;
      FixedFloatValue& operator =(const T& value);

      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Fixed 32-bit float.
     */
    class Float32Value: public FixedFloatValue<float> {
      using FixedFloatValue<float>::FixedFloatValue;
    };

    /**
     * Fixed 64-bit float.
     */
    class Float64Value: public FixedFloatValue<double> {
      using FixedFloatValue<double>::FixedFloatValue;
    };

    /**
     * Boolean.
     */
    class BoolValue: public Value {
    public:
      bool value;

    public:
      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Variable UTF-8 encoded string.
     */
    class StrValue: public Value {
    public:
      std::string value;

    public:
      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Variable or fixed length raw bytes.
     */
    class DataValue: public Value {
    public:
      std::optional<UintValue> fixed_size;
      std::vector<uint8_t> value;

    public:
      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Void.
     */
    class VoidValue: public Value {
    public:
      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Enumeration.
     */
    class EnumValue: public UintValue {
      using UintValue::UintValue;
    };

    /**
     * Optional.
     */
    class OptionalValue: public Value {
    public:
      // TODO: void value not allowed
      std::optional<std::unique_ptr<user_value_t>> value;

    public:
      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Variable or fixed length array.
     */
    class ListValue: public Value {
    public:
      // TODO: void value not allowed
      typedef std::vector<user_value_t> list_values_t;

    public:
      // TODO: must be between (1, UINT64_MAX) inclusive
      std::optional<UintValue> fixed_size;
      list_values_t values;

    public:
      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Variable length unordered map.
     */
    class MapValue: public Value {
    public:
      // TODO: keys must all be the same type
      // TODO: f32, f64, data, data[length] keys not allowed
      // TODO: void keys or values not allowed
      typedef std::unordered_map<user_value_t, user_value_t> map_values_t;

    public:
      map_values_t values;

    public:
      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Union.
     */
    class UnionValue: public Value {
    public:
      UintValue tag;
      std::unique_ptr<user_value_t> value;

    public:
      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Struct with ordered fields.
     */
    class StructValue: public Value {
    public:
      // TODO: void value not allowed
      // TODO: must contain at least one field
      typedef std::map<std::string, user_value_t> struct_fields_t;

    public:
      struct_fields_t fields;

    public:
      void encode(byte_buffer_t& buffer) const;
    };
  }
}

#pragma once

// C++ Standard Library
#include <cstddef>
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
     * Any type variant.
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
    > any_value_t;

    /**
     * Any concrete type variant.
     *
     * This includes all types except the void type.
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
      EnumValue,
      OptionalValue,
      ListValue,
      MapValue,
      UnionValue,
      StructValue
    > concrete_value_t;

    /**
     * Any type suitable as a map key variant.
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
      BoolValue,
      StrValue,
      EnumValue
    > map_key_value_t;

    /**
     * Byte buffer for storing encoded values.
     *
     * XXX: Some way to use output byte streams instead?
     * XXX: https://tuttlem.github.io/2014/08/18/getting-istream-to-work-off-a-byte-array.html
     * XXX: https://github.com/cplusplus/papers/issues/860
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
    private:
      // XXX: consider using https://gmplib.org/ to store arbitrarily large integers
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
      // XXX: consider using https://gmplib.org/ to store arbitrarily large integers
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
    private:
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
    class Int64Value: public FixedIntValue<int64_t> {
      using FixedIntValue<int64_t>::FixedIntValue;
    };

    /**
     * Abstract base class for fixed floating point values.
     */
    template <typename T>
    class FixedFloatValue: public Value {
    private:
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
    private:
      bool value;

    public:
      BoolValue();
      BoolValue(const bool& initial);

      operator bool() const;
      BoolValue& operator =(const bool& value);

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
      byte_buffer_t value;

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
      std::shared_ptr<concrete_value_t> value;

    public:
      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Variable or fixed length array.
     */
    class ListValue: public Value {
    public:
      // TODO: values must all be the same type
      typedef std::vector<concrete_value_t> list_values_t;

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
      typedef std::unordered_map<map_key_value_t, concrete_value_t> map_values_t;

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
      std::shared_ptr<any_value_t> value;

    public:
      void encode(byte_buffer_t& buffer) const;
    };

    /**
     * Struct with ordered fields.
     */
    class StructValue: public Value {
    public:
      // TODO: must contain at least one field
      typedef std::map<std::string, concrete_value_t> struct_fields_t;

    public:
      struct_fields_t fields;

    public:
      void encode(byte_buffer_t& buffer) const;
    };
  }
}

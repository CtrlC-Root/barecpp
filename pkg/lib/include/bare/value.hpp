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
     * Abstract base class for values.
     */
    class Value { };

    /**
     * Variable length unsigned integer.
     */
    class UintValue: public Value { };

    /**
     * Fixed 8-bit unsigned integer.
     */
    class Uint8Value: public Value {
    public:
      uint8_t value;
    };

    /**
     * Fixed 16-bit unsigned integer.
     */
    class Uint16Value: public Value {
    public:
      uint16_t value;
    };

    /**
     * Fixed 32-bit unsigned integer.
     */
    class Uint32Value: public Value {
    public:
      uint32_t value;
    };

    /**
     * Fixed 64-bit unsigned integer.
     */
    class Uint64Value: public Value {
    public:
      uint64_t value;
    };

    /**
     * Variable length signed integer.
     */
    class IntValue: public Value { };

    /**
     * Fixed 8-bit signed integer.
     */
    class Int8Value: public Value {
    public:
      int8_t value;
    };

    /**
     * Fixed 16-bit signed integer.
     */
    class Int16Value: public Value {
    public:
      int16_t value;
    };

    /**
     * Fixed 32-bit signed integer.
     */
    class Int32Value: public Value {
    public:
      int32_t value;
    };

    /**
     * Fixed 64-bit signed integer.
     */
    class Int64Value: public Value {
    public:
      int64_t value;
    };

    /**
     * Fixed 32-bit float.
     */
    class Float32Value: public Value {
    public:
      float value;
    };

    /**
     * Fixed 64-bit float.
     */
    class Float64Value: public Value {
    public:
      double value;
    };

    /**
     * Boolean.
     */
    class BoolValue: public Value {
    public:
      bool value;
    };

    /**
     * Variable UTF-8 encoded string.
     */
    class StrValue: public Value {
    public:
      std::string value;
    };

    /**
     * Variable or fixed length raw bytes.
     */
    class DataValue: public Value {
    public:
      std::optional<UintValue> fixed_size;
      std::vector<uint8_t> value;
    };

    /**
     * Void.
     */
    class VoidValue: public Value { };

    /**
     * Enumeration.
     */
    class EnumValue: public Value {
    public:
      UintValue value;
    };

    /**
     * Optional.
     */
    class OptionalValue: public Value {
    public:
      std::optional<std::unique_ptr<user_value_t>> value;
    };

    /**
     * Variable or fixed length array.
     */
    class ListValue: public Value {
    public:
      typedef std::vector<Value> list_values_t;

    public:
      std::optional<UintValue> fixed_size;
      list_values_t values;
    };

    /**
     * Variable length unordered map.
     */
    class MapValue: public Value {
    public:
      typedef std::unordered_map<Value, Value> map_values_t;

    public:
      map_values_t values;
    };

    /**
     * Union.
     */
    class UnionValue: public Value {
    public:
      UintValue tag;
      std::unique_ptr<user_value_t> value;
    };

    /**
     * Struct with ordered fields.
     */
    class StructValue: public Value {
    public:
      typedef std::map<std::string, user_value_t> struct_fields_t;

    public:
      struct_fields_t fields;
    };
  }
}

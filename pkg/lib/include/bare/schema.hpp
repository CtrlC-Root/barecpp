#pragma once

// C++ Standard Library
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

// BARE
#include <bare/value.hpp>

namespace bare {
  namespace schema {
    /**
     * Forward declare type classes.
     */
    class UintType;
    class Uint8Type;
    class Uint16Type;
    class Uint32Type;
    class Uint64Type;
    class IntType;
    class Int8Type;
    class Int16Type;
    class Int32Type;
    class Int64Type;
    class Float32Type;
    class Float64Type;
    class BoolType;
    class StrType;
    class DataType;
    class VoidType;
    class EnumType;
    class OptionalType;
    class ListType;
    class MapType;
    class UnionType;
    class StructType;

    /**
     * User type variant.
     */
    typedef std::variant<
      UintType,
      Uint8Type,
      Uint16Type,
      Uint32Type,
      Uint64Type,
      IntType,
      Int8Type,
      Int16Type,
      Int32Type,
      Int64Type,
      Float32Type,
      Float64Type,
      BoolType,
      StrType,
      DataType,
      VoidType,
      EnumType,
      OptionalType,
      ListType,
      MapType,
      UnionType,
      StructType
    > user_type_t;

    /**
     * Abstract base class for types.
     */
    class Type { };

    /**
     * Variable length unsigned integer.
     */
    class UintType: public Type { };

    /**
     * Fixed 8-bit unsigned integer.
     */
    class Uint8Type: public Type { };

    /**
     * Fixed 16-bit unsigned integer.
     */
    class Uint16Type: public Type { };

    /**
     * Fixed 32-bit unsigned integer.
     */
    class Uint32Type: public Type { };

    /**
     * Fixed 64-bit unsigned integer.
     */
    class Uint64Type: public Type { };

    /**
     * Variable length signed integer.
     */
    class IntType: public Type { };

    /**
     * Fixed 8-bit signed integer.
     */
    class Int8Type: public Type { };

    /**
     * Fixed 16-bit signed integer.
     */
    class Int16Type: public Type { };

    /**
     * Fixed 32-bit signed integer.
     */
    class Int32Type: public Type { };

    /**
     * Fixed 64-bit signed integer.
     */
    class Int64Type: public Type { };

    /**
     * Fixed 32-bit float.
     */
    class Float32Type: public Type { };

    /**
     * Fixed 64-bit float.
     */
    class Float64Type: public Type { };

    /**
     * Boolean.
     */
    class BoolType: public Type { };

    /**
     * Variable UTF-8 encoded string.
     */
    class StrType: public Type { };

    /**
     * Variable or fixed length raw bytes.
     */
    class DataType: public Type {
    public:
      std::optional<bare::value::UintValue> fixed_size;
    };

    /**
     * Void.
     */
    class VoidType: public Type { };

    /**
     * Enumeration.
     */
    class EnumType: public Type {
    public:
      typedef std::unordered_map<
        std::string,
        bare::value::UintValue
      > enum_values_t;

    public:
      enum_values_t values;
    };

    /**
     * Optional.
     */
    class OptionalType: public Type {
    public:
      std::unique_ptr<user_type_t> inner_type;
    };

    /**
     * Variable or fixed length array.
     */
    class ListType: public Type {
    public:
      std::optional<bare::value::UintValue> fixed_size;
      std::unique_ptr<user_type_t> inner_type;
    };

    /**
     * Variable length unordered map.
     */
    class MapType: public Type {
    public:
      std::unique_ptr<user_type_t> key_type;
      std::unique_ptr<user_type_t> value_type;
    };

    /**
     * Union.
     */
    class UnionType: public Type {
    public:
      typedef std::unordered_map<
        user_type_t,
        bare::value::UintValue
      > union_members_t;

    public:
      union_members_t members;
    };

    /**
     * Struct with ordered fields.
     */
    class StructType: public Type {
    public:
      typedef std::map<std::string, user_type_t> struct_fields_t;

    public:
      struct_fields_t fields;
    };

    /**
     * Schema containing multiple named user types.
     */
    class Schema {
    public:
      typedef std::unordered_map<std::string, user_type_t> user_type_map_t;

    private:
      user_type_map_t user_types;
    };
  }
}

#include <bare/schema.hpp>

using namespace bare::schema;

/**
 * FixedIntType
 */

template class FixedIntType<uint8_t>;
template class FixedIntType<uint16_t>;
template class FixedIntType<uint32_t>;
template class FixedIntType<uint64_t>;
template class FixedIntType<int8_t>;
template class FixedIntType<int16_t>;
template class FixedIntType<int32_t>;
template class FixedIntType<int64_t>;

/**
 * FixedFloatType
 */

template class FixedFloatType<float>;
template class FixedFloatType<double>;

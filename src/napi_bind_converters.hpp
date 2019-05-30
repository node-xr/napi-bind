#pragma once
#include <cstring>
#include <node_api.h>
#include <type_traits>

namespace napi_bind
{

//===========================================================================
template <>
inline std::string decode(napi_env env, napi_value value)
{
  constexpr size_t buffer_len = 2048;
  char buffer[buffer_len];

  // FIXME: this is doing an extra copy.
  size_t result_len;
  ok(env, napi_get_value_string_utf8(env, value, buffer, buffer_len, &result_len));
  std::string result(buffer);

  return result;
}

template <>
inline napi_value encode(napi_env env, const std::string value)
{
  napi_value result;
  ok(env, napi_create_string_utf8(env, value.c_str(), value.length(), &result));
  return result;
}

//===========================================================================
template <>
inline napi_value encode(napi_env env, const char *value)
{
  napi_value result;
  if (value)
  {
    ok(env, napi_create_string_utf8(env, value, strlen(value), &result));
  }
  else
  {
    ok(env, napi_get_null(env, &result));
  }
  return result;
}

//===========================================================================
template <>
inline bool decode(napi_env env, napi_value value)
{
  bool result;
  ok(env, napi_get_value_bool(env, value, &result));
  return result;
}

template <>
inline napi_value encode(napi_env env, const bool value)
{
  napi_value result;
  ok(env, napi_get_boolean(env, value, &result));
  return result;
}

//===========================================================================
template <>
inline uint8_t decode(napi_env env, napi_value value)
{
  uint32_t result;
  ok(env, napi_get_value_uint32(env, value, &result));
  return static_cast<uint8_t>(result);
}

template <>
inline napi_value encode(napi_env env, const uint8_t value)
{
  napi_value result;
  ok(env, napi_create_uint32(env, static_cast<uint32_t>(value), &result));
  return result;
}

//===========================================================================
template <>
inline uint16_t decode(napi_env env, napi_value value)
{
  uint32_t result;
  ok(env, napi_get_value_uint32(env, value, &result));
  return static_cast<uint16_t>(result);
}

template <>
inline napi_value encode(napi_env env, const uint16_t value)
{
  napi_value result;
  ok(env, napi_create_uint32(env, static_cast<uint16_t>(value), &result));
  return result;
}

//===========================================================================
template <>
inline uint32_t decode(napi_env env, napi_value value)
{
  uint32_t result;
  ok(env, napi_get_value_uint32(env, value, &result));
  return result;
}

template <>
inline napi_value encode(napi_env env, const uint32_t value)
{
  napi_value result;
  ok(env, napi_create_uint32(env, value, &result));
  return result;
}

//===========================================================================
template <>
inline uint64_t decode(napi_env env, napi_value value)
{
  napi_valuetype type;
  ok(env, napi_typeof(env, value, &type));

  uint64_t result;
  bool lossless;

  switch (type)
  {
  case napi_bigint:
    ok(env, napi_get_value_bigint_uint64(env, value, &result, &lossless));
    if (!lossless)
      throw new std::runtime_error("BigInt to Uint64 conversion loss.");
    break;

  case napi_number:
    ok(env, napi_get_value_int64(env, value, reinterpret_cast<int64_t *>(&result)));
    break;

  default:
    throw std::runtime_error("Invalid JS type for uint64_t.");
  }

  return result;
}

//===========================================================================
template <>
inline int8_t decode(napi_env env, napi_value value)
{
  int32_t result;
  ok(env, napi_get_value_int32(env, value, &result));
  return result;
}

template <>
inline napi_value encode(napi_env env, const int8_t value)
{
  napi_value result;
  ok(env, napi_create_int32(env, value, &result));
  return result;
}

//===========================================================================
template <>
inline int16_t decode(napi_env env, napi_value value)
{
  int32_t result;
  ok(env, napi_get_value_int32(env, value, &result));
  return result;
}

template <>
inline napi_value encode(napi_env env, const int16_t value)
{
  napi_value result;
  ok(env, napi_create_int32(env, value, &result));
  return result;
}

//===========================================================================
template <>
inline int32_t decode(napi_env env, napi_value value)
{
  int32_t result;
  ok(env, napi_get_value_int32(env, value, &result));
  return result;
}

template <>
inline napi_value encode(napi_env env, const int32_t value)
{
  napi_value result;
  ok(env, napi_create_int32(env, value, &result));
  return result;
}

//===========================================================================
template <>
inline int64_t decode(napi_env env, napi_value value)
{
  napi_valuetype type;
  ok(env, napi_typeof(env, value, &type));

  int64_t result;
  bool lossless;

  switch (type)
  {
  case napi_bigint:
    ok(env, napi_get_value_bigint_int64(env, value, &result, &lossless));
    if (!lossless)
      throw new std::runtime_error("BigInt to Uint64 conversion loss.");
    break;

  case napi_number:
    ok(env, napi_get_value_int64(env, value, &result));
    break;

  default:
    throw std::runtime_error("Invalid JS type for uint64_t.");
  }

  return result;
}

template <>
inline napi_value encode(napi_env env, const int64_t value)
{
  napi_value result;
  ok(env, napi_create_bigint_int64(env, value, &result));
  return result;
}

template <>
inline napi_value encode(napi_env env, const uint64_t value)
{
  napi_value result;
  ok(env, napi_create_bigint_uint64(env, value, &result));
  return result;
}

//===========================================================================
// Default for unmanaged pointer types.
template <typename T>
struct decoder<T, typename std::enable_if_t<std::is_pointer<T>::value>>
{
  static T eval(napi_env env, napi_value value)
  {
    void *result;
    ok(env, napi_get_value_external_opt(env, value, &result));
    return static_cast<T>(result);
  }
};

template <typename T>
struct encoder<T, typename std::enable_if_t<std::is_pointer<T>::value>>
{
  static napi_value eval(napi_env env, T value)
  {
    napi_value result;
    // For a default pointer handler, assume memory is not owned.
    napi_create_external_opt(env, static_cast<void *>(value), nullptr, nullptr, &result);
    return result;
  }
};

//===========================================================================
// Default for floating-point types.
template <typename T>
struct decoder<T, typename std::enable_if_t<std::is_floating_point<T>::value>>
{
  static T eval(napi_env env, napi_value value)
  {
    double result;
    ok(env, napi_get_value_double(env, value, &result));
    return static_cast<T>(result);
  }
};

template <typename T>
struct encoder<T, typename std::enable_if_t<std::is_floating_point<T>::value>>
{
  static napi_value eval(napi_env env, T value)
  {
    napi_value result;
    ok(env, napi_create_double(env, static_cast<double>(value), &result));
    return result;
  }
};

//===========================================================================
// Default for enum types.
template <typename T>
struct decoder<T, typename std::enable_if_t<std::is_enum<T>::value>>
{
  static T eval(napi_env env, napi_value value)
  {
    uint32_t result;
    ok(env, napi_get_value_uint32(env, value, &result));
    return static_cast<T>(result);
  }
};

template <typename T>
struct encoder<T, typename std::enable_if_t<std::is_enum<T>::value>>
{
  static napi_value eval(napi_env env, T value)
  {
    napi_value result;
    ok(env, napi_create_uint32(env, static_cast<uint32_t>(value), &result));
    return result;
  }
};

} // namespace napi_bind
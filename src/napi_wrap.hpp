#pragma once
#include <node_api.h>
#include <stdexcept>

namespace napi_wrap
{

inline constexpr auto ok(napi_status status) -> void
{
  // https://stackoverflow.com/a/35349979
  (status != napi_ok) ? throw std::runtime_error("Failed N-API call.") : 0;
}

//===========================================================================

// Specialize this function to implement a custom decoder for a single type.
template <typename T>
T decode(napi_env env, napi_value value);

// Specialize this function to implement a custom encoder for a single type.
template <typename T>
napi_value encode(napi_env env, const T value);

// Specialize this class to implement a custom decoder over all types matching a `std::enable_if` predicate.
template <typename T, typename Predicate>
struct decoder;

// Specialize this class to implement a custom encoder over all types matching a `std::enable_if` predicate.
template <typename T, typename Predicate>
struct encoder;

//===========================================================================

// Decode a property of a javascript object.
template <typename T>
T decode_property(napi_env env, napi_value object, const char *prop);

// Decode a property of a javascript object or return a default value.
template <typename T>
T decode_property(napi_env env, napi_value object, const char *prop, T default_value);

// Encode a property and store it in a javascript object.
// Specialize this function to implement a custom encoder for a single type.
template <typename T>
void encode_property(napi_env env, napi_value object, const char *prop, const T value);

//===========================================================================

template <typename Result, typename... Args>
napi_status set_function(napi_env env, napi_value parent, const char *name, Result (*fn)(Args...));

} // namespace napi_wrap

// Helper function for N-API that supports C++ nullptr -> JS null aliasing.
napi_status napi_get_value_external_opt(napi_env env, napi_value value, void **result);
napi_status napi_create_external_opt(napi_env env, void *data, napi_finalize finalize_cb, void *finalize_hint, napi_value *result);

#include "napi_wrap_detail.hpp"
#include "napi_wrap_converters.hpp"

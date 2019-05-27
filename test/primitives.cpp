#include "primitives.hpp"
#include "util.hpp"
#include <napi_bind.hpp>

using napi_bind::ok;
using napi_bind::set_function;

napi_value create_primitives(napi_env env)
{
  napi_value value;
  ok(env, napi_create_object(env, &value));

  set_function(env, value, "identity_bool", identity<bool>);

  set_function(env, value, "identity_int8", identity<int8_t>);
  set_function(env, value, "identity_int16", identity<int16_t>);
  set_function(env, value, "identity_int32", identity<int32_t>);
  set_function(env, value, "identity_int64", identity<int64_t>);

  set_function(env, value, "identity_uint8", identity<uint8_t>);
  set_function(env, value, "identity_uint16", identity<uint16_t>);
  set_function(env, value, "identity_uint32", identity<uint32_t>);
  set_function(env, value, "identity_uint64", identity<uint64_t>);

  set_function(env, value, "identity_float", identity<float>);
  set_function(env, value, "identity_double", identity<double>);

  return value;
}

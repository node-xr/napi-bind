#include "structs.hpp"
#include "util.hpp"
#include <napi_bind.hpp>

using napi_bind::ok;
using napi_bind::set_function;

namespace napi_bind
{

//===========================================================================
template <>
inline napi_value encode(napi_env env, test_embedded_t obj)
{
  napi_value result;
  ok(env, napi_create_object(env, &result));

  encode_property(env, result, "field_nested", obj.field_nested);

  return result;
}

template <>
inline test_embedded_t decode(napi_env env, napi_value value)
{
  test_embedded_t result;
  result.field_nested = decode_property<uint32_t>(env, value, "field_nested");
  return result;
}

//===========================================================================
template <>
inline napi_value encode(napi_env env, test_object_t obj)
{
  napi_value result;
  ok(env, napi_create_object(env, &result));

  encode_property<uint32_t>(env, result, "field", obj.field);
  encode_property<uint32_t>(env, result, "field_default", obj.field_default);
  encode_property<test_embedded_t>(env, result, "field_embedded", obj.field_embedded);
  //encode_property<std::optional<uint32_t>>(env, result, "field_optional_absent", obj.field_optional_absent);
  encode_property<std::optional<uint32_t>>(env, result, "field_optional_present", obj.field_optional_present);

  return result;
}

template <>
inline test_object_t decode(napi_env env, napi_value value)
{
  test_object_t result;
  result.field = decode_property<uint32_t>(env, value, "field");
  result.field_default = decode_property<uint32_t>(env, value, "field_default", 5555);
  result.field_embedded = decode_property<test_embedded_t>(env, value, "field_embedded");
  result.field_optional_absent = decode_property<std::optional<uint32_t>>(env, value, "field_optional_absent");
  result.field_optional_present = decode_property<std::optional<uint32_t>>(env, value, "field_optional_present");
  return result;
}

} // namespace napi_bind

napi_value create_structs(napi_env env)
{
  napi_value value;
  ok(env, napi_create_object(env, &value));

  set_function(env, value, "identity_struct", identity<test_object_t>);

  return value;
}
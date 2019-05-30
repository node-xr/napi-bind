#include "enums.hpp"
#include "util.hpp"
#include <napi_bind.hpp>

using napi_bind::ok;
using napi_bind::set_function;

enum unscoped_enum_t
{
  A = 0,
  B = 1,
  C = 2,
  // Intentional gap
  E = 4,
  Count
};

enum class scoped_enum_t
{
  red = 20,
  green,
  blue
};

bool is_B(unscoped_enum_t value)
{
  return value == unscoped_enum_t::B;
}

bool is_red(scoped_enum_t value)
{
  return value == scoped_enum_t::red;
}

napi_value
create_enums(napi_env env)
{
  napi_value value;
  ok(env, napi_create_object(env, &value));

  set_function(env, value, "identity_unscoped", identity<unscoped_enum_t>);
  set_function(env, value, "identity_scoped", identity<scoped_enum_t>);
  set_function(env, value, "is_B", is_B);
  set_function(env, value, "is_red", is_red);

  return value;
}
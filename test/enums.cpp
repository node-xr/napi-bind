#include "enums.hpp"
#include "util.hpp"
#include <napi_bind.hpp>

using napi_bind::ok;
using napi_bind::set_function;

enum test_enum_t
{
    A=0,
    B=1,
    C=2,
    // Intentional gap
    E=4,
    Count
};

napi_value create_enums(napi_env env)
{
  napi_value value;
  ok(env, napi_create_object(env, &value));

  set_function(env, value, "identity_enum", identity<test_enum_t>);

  return value;
}
#include "strings.hpp"
#include <napi_bind.hpp>

using napi_bind::ok;
using napi_bind::set_function;

napi_value create_strings(napi_env env)
{
  napi_value value;
  ok(napi_create_object(env, &value));
  return value;
}
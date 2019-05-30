#include "strings.hpp"
#include "util.hpp"
#include <napi_bind.hpp>
#include <string>

using napi_bind::ok;
using napi_bind::set_function;

std::string identity_str(std::string value)
{
  return value;
}

const std::string identity_str_ref(const std::string &value)
{
  return value;
}

const char *identity_char_ptr(const char *value)
{
  return value;
}

napi_value create_strings(napi_env env)
{
  napi_value value;
  ok(env, napi_create_object(env, &value));

  set_function(env, value, "identity_str", identity_str);
  set_function(env, value, "identity_str_ref", identity_str_ref);
  set_function(env, value, "identity_char_ptr", identity_char_ptr);

  return value;
}

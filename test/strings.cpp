#include "strings.hpp"
#include "util.hpp"
#include <napi_bind.hpp>
#include <string>

using napi_bind::ok;
using napi_bind::set_function;

std::string identity_str1(std::string value)
{
  return value;
}

const std::string identity_str2(const std::string &value)
{
  return value;
}

const char *identity_str3(const char *value)
{
  return value;
}

napi_value create_strings(napi_env env)
{
  napi_value value;
  ok(napi_create_object(env, &value));

  //set_function(env, value, "identity_str1", identity_str1);
  //set_function(env, value, "identity_str2", identity_str2);
  //set_function(env, value, "identity_str3", identity_str3);

  return value;
}

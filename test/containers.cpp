#include "containers.hpp"
#include "util.hpp"
#include <list>
#include <napi_bind.hpp>
#include <optional>
#include <vector>

using napi_bind::ok;
using napi_bind::set_function;

napi_value create_containers(napi_env env)
{
  napi_value value;
  ok(env, napi_create_object(env, &value));

  set_function(env, value, "identity_list", identity<const std::list<std::string>>);
  set_function(env, value, "identity_optional", identity<const std::optional<std::string>>);
  set_function(env, value, "identity_vector", identity<const std::vector<std::string>>);

  return value;
}
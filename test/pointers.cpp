#include "pointers.hpp"
#include "util.hpp"
#include <napi_bind.hpp>

using napi_bind::ok;
using napi_bind::set_function;

struct test_struct {
  uint16_t payload;
};

napi_value create_pointers(napi_env env)
{
  napi_value value;
  ok(napi_create_object(env, &value));

  set_function(env, value, "identity_void_ptr", identity<void *>);
  set_function(env, value, "identity_test_ptr", identity<test_struct *>);

  return value;
}
#include "pointers.hpp"
#include "util.hpp"
#include <napi_bind.hpp>

using napi_bind::ok;
using napi_bind::set_function;

struct test_struct
{
  uint16_t payload;
};

test_struct *create_struct_ptr()
{
  return new test_struct();
}

void *create_void_ptr()
{
  return create_struct_ptr();
}

void *create_null_ptr()
{
  return nullptr;
}

void delete_struct_ptr(test_struct *ptr)
{
  delete ptr;
}

napi_value create_pointers(napi_env env)
{
  napi_value value;
  ok(env, napi_create_object(env, &value));

  set_function(env, value, "create_struct_ptr", create_struct_ptr);
  set_function(env, value, "create_void_ptr", create_void_ptr);
  set_function(env, value, "create_null_ptr", create_null_ptr);
  set_function(env, value, "delete_struct_ptr", delete_struct_ptr);
  set_function(env, value, "identity_void_ptr", identity<void *>);
  set_function(env, value, "identity_test_ptr", identity<test_struct *>);

  return value;
}

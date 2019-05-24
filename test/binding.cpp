#include "node_api.h"
#include "primitives.hpp"

using napi_bind::ok;

static napi_value Init(napi_env env, napi_value exports)
{
  ok(napi_set_named_property(env, create_primitives(env), "primitives", &exports));
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
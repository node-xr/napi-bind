#include <node_api.h>
#include <napi_bind.hpp>
#include "primitives.hpp"

using napi_bind::ok;

static napi_value Init(napi_env env, napi_value exports)
{
  ok(napi_set_named_property(env, exports, "primitives", create_primitives(env)));
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);

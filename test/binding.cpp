#include <node_api.h>
#include <napi_bind.hpp>
#include "enums.hpp"
#include "pointers.hpp"
#include "primitives.hpp"
#include "strings.hpp"

using napi_bind::ok;

static napi_value Init(napi_env env, napi_value exports)
{
  ok(env, napi_set_named_property(env, exports, "enums", create_enums(env)));
  ok(env, napi_set_named_property(env, exports, "pointers", create_pointers(env)));
  ok(env, napi_set_named_property(env, exports, "primitives", create_primitives(env)));
  ok(env, napi_set_named_property(env, exports, "strings", create_strings(env)));
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);

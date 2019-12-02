#include "containers.hpp"
#include "enums.hpp"
#include "pointers.hpp"
#include "primitives.hpp"
#include "strings.hpp"
#include "structs.hpp"
#include <napi_bind.hpp>
#include <node_api.h>

using napi_bind::ok;

static napi_value Init(napi_env env, napi_value exports)
{
  ok(env, napi_set_named_property(env, exports, "containers", create_containers(env)));
  ok(env, napi_set_named_property(env, exports, "enums", create_enums(env)));
  ok(env, napi_set_named_property(env, exports, "pointers", create_pointers(env)));
  ok(env, napi_set_named_property(env, exports, "primitives", create_primitives(env)));
  ok(env, napi_set_named_property(env, exports, "strings", create_strings(env)));
  ok(env, napi_set_named_property(env, exports, "structs", create_structs(env)));
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

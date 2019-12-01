#pragma once
#include <node_api.h>
#include <optional>

struct test_embedded_t
{
  uint32_t field_nested;
};

struct test_object_t
{
  uint32_t field;
  uint32_t field_default;
  test_embedded_t field_embedded;
  std::optional<uint32_t> field_optional_absent;
  std::optional<uint32_t> field_optional_present;
};

napi_value create_structs(napi_env env);
#pragma once
#include <array>
#include <node_api.h>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace napi_wrap
{

// https://stackoverflow.com/a/47640807
template <typename T, typename = void>
struct decoder
{
  static T eval(napi_env env, napi_value value)
  {
    static_assert(sizeof(T) == 0, "No decoder implemented.");
    throw std::invalid_argument("No decoder implemented.");
  }
};

template <typename T, typename = void>
struct encoder
{
  static napi_value eval(napi_env env, T value)
  {
    static_assert(sizeof(T) == 0, "No encoder implemented.");
    throw std::invalid_argument("No encoder implemented.");
  }
};

template <typename T>
T decode(napi_env env, napi_value value)
{
  return decoder<T>::eval(env, value);
}

template <typename T>
napi_value encode(napi_env env, const T value)
{
  return encoder<T>::eval(env, value);
}

template <typename T>
T decode_idx(napi_env env, napi_value *argv, size_t idx)
{
  return decode<T>(env, argv[idx]);
}

template <typename T>
T decode_property(napi_env env, napi_value object, const char *prop)
{
  napi_value result;
  ok(napi_get_named_property(env, object, prop, &result));
  return decode<T>(env, result);
}

template <typename T>
T decode_property(napi_env env, napi_value object, const char *prop, T default_value)
{
  bool has_property;
  ok(napi_has_named_property(env, object, prop, &has_property));

  if (has_property) {
    return decode_property<T>(env, object, prop);
  } else {
    return default_value;
  }
}

template <typename T>
void encode_property(napi_env env, napi_value object, const char *prop, const T value)
{
  napi_value result = encode<T>(env, value);
  ok(napi_set_named_property(env, object, prop, result));
}

// https://blog.tartanllama.xyz/exploding-tuples-fold-expressions/
template <typename Result, typename... Args, std::size_t... Idx>
napi_value caller(Result (*fn)(Args...), napi_env env, napi_value *argv, std::index_sequence<Idx...>)
{
  // Prevent unused argument warning.
  (void)argv;

  try
  {
    return encode<Result>(env, fn(decode_idx<Args>(env, argv, Idx)...));
  }
  catch (std::exception &e)
  {
    napi_throw_error(env, nullptr, e.what());
    return nullptr;
  }
}

// Specialization for the void return type.
template <typename Result = void, typename... Args, std::size_t... Idx>
napi_value caller(void (*fn)(Args...), napi_env env, napi_value *argv, std::index_sequence<Idx...>)
{
  // Prevent unused argument warning.
  (void)argv;

  try
  {
    fn(decode_idx<Args>(env, argv, Idx)...);
    return nullptr;
  }
  catch (std::exception &e)
  {
    napi_throw_error(env, nullptr, e.what());
    return nullptr;
  }
}

template <typename Result, typename... Args>
napi_value wrapper(napi_env env, napi_callback_info info)
{
  constexpr auto num_args = sizeof...(Args);
  Result (*fn)(Args...);

  // Retrieve arguments for callback.
  auto argv = std::array<napi_value, sizeof...(Args)>{};
  size_t argc = num_args;
  ok(napi_get_cb_info(env, info, &argc, argv.data(), nullptr, (void **)&fn));

  // Check that the correct number of arguments was received.
  if (argc < num_args)
  {
    std::ostringstream ss;
    ss << "argument length mismatch: expected " << num_args << ", received " << argc;
    napi_throw_error(env, nullptr, ss.str().c_str());
    return nullptr;
  }

  // Check that the function pointer was correctly loaded.
  if (!fn)
  {
    napi_throw_error(env, nullptr, "Unable to retrieve function.");
    return nullptr;
  }

  // Convert of the arguments using encode and decode.
  // https://stackoverflow.com/a/4931251
  // https://stackoverflow.com/a/37100646
  return caller(fn, env, argv.data(), std::index_sequence_for<Args...>{});
}

template <typename Result, typename... Args>
napi_status set_function(napi_env env, napi_value parent, const char *name, Result (*fn)(Args...))
{
  napi_status status = napi_ok;
  napi_value exported_fn = nullptr;

  // Create a captureless lambda that preserves type information.
  auto wrapper_fn = [](napi_env env, napi_callback_info info) {
    return wrapper<Result, Args...>(env, info);
  };

  // Create a function wrapper that will auto-convert arguments.
  status = napi_create_function(env, name, NAPI_AUTO_LENGTH, wrapper_fn, (void *)fn, &exported_fn);
  if (status != napi_ok)
  {
    std::ostringstream ss;
    ss << "Unable to wrap native function: " << name;
    napi_throw_error(env, nullptr, ss.str().c_str());
    return napi_pending_exception;
  }

  // Set the function wrapper on the provided export object.
  status = napi_set_named_property(env, parent, name, exported_fn);
  if (status != napi_ok)
  {
    std::ostringstream ss;
    ss << "Unable to set property with: " << name;
    napi_throw_error(env, nullptr, ss.str().c_str());
    return napi_pending_exception;
  }

  return napi_ok;
}

} // namespace napi_wrap

inline napi_status napi_get_value_external_opt(napi_env env, napi_value value, void **result)
{
  napi_status status;
  napi_valuetype type;

  // Check if this argument is set to null.
  status = napi_typeof(env, value, &type);
  if (status != napi_ok)
  {
    return status;
  }

  if (type == napi_null)
  {
    // If null was provided, fill in a null-pointer.
    *result = nullptr;
    return napi_ok;
  }
  else
  {
    // Otherwise, load the actual external pointer.
    status = napi_get_value_external(env, value, result);
    return status;
  }
}

inline napi_status napi_create_external_opt(napi_env env, void *data, napi_finalize finalize_cb, void *finalize_hint, napi_value *result)
{
  return (data) ? napi_create_external(env, data, finalize_cb, finalize_hint, result) : napi_get_null(env, result);
}

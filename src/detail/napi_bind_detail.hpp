#pragma once
#include <array>
#include <node_api.h>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace napi_bind
{

// https://stackoverflow.com/a/47640807
template <typename T, typename = void>
struct decoder
{
  decoder(napi_env e, napi_value v) : env(e), value(v) {}
  ~decoder() {}

  T operator()()
  {
    static_assert(sizeof(T) == 0, "No decoder implemented.");
    throw std::invalid_argument("No decoder implemented.");
  }

private:
  napi_env env;
  napi_value value;
};

template <typename T, typename = void>
struct encoder
{
  encoder(T v) : value(v) {}
  ~encoder() {}

  napi_value operator()(napi_env)
  {
    static_assert(sizeof(T) == 0, "No encoder implemented.");
    throw std::invalid_argument("No encoder implemented.");
  }

private:
  T value;
};

//===========================================================================
// Based on: https://vittorioromeo.info/index/blog/checking_expression_validity_in_place.html

template <typename...>
using void_t = void;

template <typename, typename = void>
struct has_decode : std::false_type
{
};

template <typename T>
struct has_decode<T, void_t<decltype(decode<T>(std::declval<napi_env>(), std::declval<napi_value>()))>> : std::true_type
{
};

template <typename, typename = void>
struct has_encode : std::false_type
{
};

template <typename T>
struct has_encode<T, void_t<decltype(encode<T>(std::declval<napi_env>(), std::declval<T>()))>> : std::true_type
{
};

//===========================================================================

// Based on https://stackoverflow.com/a/47640807
template <typename T>
struct decoder<T, typename std::enable_if_t<has_decode<T>{}>>
{
  decoder(napi_env e, napi_value v) : env(e), value(v) {}
  ~decoder() {}

  T operator()()
  {
    return decode<T>(env, value);
  }

private:
  napi_env env;
  napi_value value;
};

template <typename T>
struct encoder<T, typename std::enable_if_t<has_encode<T>{}>>
{
  encoder(T v) : value(v) {}
  ~encoder() {}

  napi_value operator()(napi_env env)
  {
    return encode<T>(env, value);
  }

private:
  T value;
};

template <typename T>
T decode_property(napi_env env, napi_value object, const char *prop)
{
  napi_value result;
  ok(env, napi_get_named_property(env, object, prop, &result));
  return decoder<T>(env, result)();
}

template <typename T>
T decode_property(napi_env env, napi_value object, const char *prop, T default_value)
{
  bool has_property;
  ok(env, napi_has_named_property(env, object, prop, &has_property));

  if (has_property)
  {
    return decode_property<T>(env, object, prop);
  }
  else
  {
    return default_value;
  }
}

template <typename T>
void encode_property(napi_env env, napi_value object, const char *prop, T value)
{
  napi_value result = encoder<T>(value)(env);
  if (result)
  {
    ok(env, napi_set_named_property(env, object, prop, result));
  }
  // Note: this will not _unset_ a property if undefined is passed.
  // However, it will safely skip modification of the field, so it
  // can be used during construction of new objects for optional fields.
}

template <typename T>
decoder<T> decode_idx(napi_env env, napi_value *argv, size_t idx)
{
  return decoder<T>(env, argv[idx]);
}

// https://blog.tartanllama.xyz/exploding-tuples-fold-expressions/
template <typename Result, typename... Args, std::size_t... Idx>
napi_value caller(Result (*fn)(Args...), napi_env env, napi_value *argv, std::index_sequence<Idx...>)
{
  using decoder_t = std::tuple<decoder<std::decay_t<Args>>...>;
  using encoder_t = encoder<std::decay_t<Result>>;

  // Prevent unused argument warnings.
  (void)argv;

  try
  {
    // Decode and encode in separate steps to allow RAII
    // to handle the lifecycles of these transient args.
    decoder_t decoders(decode_idx<std::decay_t<Args>>(env, argv, Idx)...);
    (void)decoders;

    // Perform wrapped function call and encode result.
    Result result = fn(std::get<Idx>(decoders)()...);
    encoder_t encoder(result);
    return encoder(env);
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
  using decoder_t = std::tuple<decoder<std::decay_t<Args>>...>;

  // Prevent unused argument warnings.
  (void)argv;

  try
  {
    // Decode and encode in separate steps to allow RAII
    // to handle the lifecycles of these transient args.
    decoder_t decoders(decode_idx<std::decay_t<Args>>(env, argv, Idx)...);
    (void)decoders;

    // Perform wrapped function call and return void result.
    fn(std::get<Idx>(decoders)()...);
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
  ok(env, napi_get_cb_info(env, info, &argc, argv.data(), nullptr, (void **)&fn));

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

} // namespace napi_bind

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

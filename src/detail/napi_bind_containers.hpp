#pragma once
#include <cstring>
#include <list>
#include <node_api.h>
#include <optional>
#include <type_traits>
#include <vector>
#include <queue>
#include <deque>

namespace napi_bind
{

//===========================================================================
// Default for std::optional types.
template <typename T>
struct decoder<std::optional<T>, typename std::enable_if_t<!has_decode<std::optional<T>>{}>>
{
  decoder(napi_env e, napi_value v)
  {
    napi_valuetype type;

    // Check the type of the input argument.
    ok(e, napi_typeof(e, v, &type));

    // If the type is truthy, instantiate a decoder for the wrapped type.
    if (type != napi_null && type != napi_undefined)
    {
      decoder_ = std::make_optional<decoder<T>>(e, v);
    }
  }

  std::optional<T> operator()()
  {
    return (decoder_) ? std::make_optional<T>((*decoder_)()) : std::nullopt;
  }

private:
  std::optional<decoder<T>> decoder_;
};

template <typename T>
struct encoder<std::optional<T>, typename std::enable_if_t<!has_encode<std::optional<T>>{}>>
{
  encoder(std::optional<T> v)
  {
    if (v)
    {
      encoder_ = std::make_optional<encoder<T>>(*v);
    }
  }

  napi_value operator()(napi_env env)
  {
    return (encoder_) ? (*encoder_)(env) : nullptr;
  }

private:
  std::optional<encoder<T>> encoder_;
};

//===========================================================================
// Default for 'container-like' std:: types.
template <class T>
struct is_container
{
  using type = T;
  constexpr static bool value = false;
};

template <class T>
struct is_container<std::vector<T>>
{
  using type = std::vector<T>;
  using element_type = T;
  constexpr static bool value = true;
};

template <class T>
struct is_container<std::list<T>>
{
  using type = std::list<T>;
  using element_type = T;
  constexpr static bool value = true;
};

template <class T>
struct is_container<std::queue<T>>
{
  using type = std::queue<T>;
  using element_type = T;
  constexpr static bool value = true;
};

template <class T>
struct is_container<std::deque<T>>
{
  using type = std::deque<T>;
  using element_type = T;
  constexpr static bool value = true;
};

template <typename T>
struct decoder<T, typename std::enable_if_t<(is_container<T>::value && !has_decode<T>{})>>
{
  using element_type = typename is_container<T>::element_type;

  decoder(napi_env e, napi_value v) : env(e), value(v) {}

  T operator()()
  {
    uint32_t length;
    ok(env, napi_get_array_length(env, value, &length));

    T result;
    for (size_t idx = 0; idx < length; ++idx)
    {
      // TODO: should the decoders be preallocated?
      napi_value elem;
      ok(env, napi_get_element(env, value, idx, &elem));
      result.push_back(decoder<element_type>(env, elem)());
    }
    return result;
  }

private:
  napi_env env;
  napi_value value;
};

template <typename T>
struct encoder<T, typename std::enable_if_t<(is_container<T>::value && !has_encode<T>{})>>
{
  using element_type = typename is_container<T>::element_type;

  encoder(T v) : value(v) {}

  napi_value operator()(napi_env env)
  {
    napi_value result;
    ok(env, napi_create_array_with_length(env, value.size(), &result));

    size_t idx = 0;
    for (const auto elem : value)
    {
      // TODO: should the encoders be preallocated?
      ok(env, napi_set_element(env, result, idx++, encoder<element_type>(elem)(env)));
    }

    return result;
  }

private:
  T value;
};

} // namespace napi_bind

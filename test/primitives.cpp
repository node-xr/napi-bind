#include <napi_wrap.hpp>
#include <string>

/// A simple identity function to test serdes.
template<typename T>
T identity(const T a)
{
    return a;
}

void setup(napi_env env, napi_value value)
{
    napi_wrap::setup_function(env, value, "identity_bool", identity<bool>);

    napi_wrap::setup_function(env, value, "identity_int8", identity<int8_t>);
    napi_wrap::setup_function(env, value, "identity_int16", identity<int16_t>);
    napi_wrap::setup_function(env, value, "identity_int32", identity<int32_t>);
    napi_wrap::setup_function(env, value, "identity_int64", identity<int64_t>);

    napi_wrap::setup_function(env, value, "identity_uint8", identity<uint8_t>);
    napi_wrap::setup_function(env, value, "identity_uint16", identity<uint16_t>);
    napi_wrap::setup_function(env, value, "identity_uint32", identity<uint32_t>);
    napi_wrap::setup_function(env, value, "identity_uint64", identity<uint64_t>);

    napi_wrap::setup_function(env, value, "identity_float", identity<float>);
    napi_wrap::setup_function(env, value, "identity_double", identity<double>);

    napi_wrap::setup_function(env, value, "identity_char", identity<char *>);
    napi_wrap::setup_function(env, value, "identity_string", identity<std::string>);
}

NAPI_MODULE(setup);

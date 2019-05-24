# napi-wrap

`napi-wrap` is a header-only C++ library that takes advantage of two things:

- C++ can do template deduction
- C functions have relatively simple types

By combining these, `napi-wrap` is able to automatically wrap C functions into N-API bindings with minimal fuss, and no external dependencies or libraries.

---

Note that while this extends to simple C++ functions, `napi-wrap` isn't really designed to do more complex wrapping of C++ objects. You might be able to get it to do so with some cleverness, but no guarantees.

If you are looking to create more complex C++ bindings, consider one of the following two libraries:

- https://github.com/nodejs/node-addon-api
- https://github.com/charto/nbind

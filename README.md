# napi-bind

[![npm version](https://badge.fury.io/js/napi-bind.svg)](https://badge.fury.io/js/napi-bind)

`napi-bind` is a header-only C++ library that takes advantage of two things:

- C++ can do template deduction
- C functions have relatively simple types

By combining these, `napi-bind` is able to automatically wrap C functions into N-API bindings with minimal fuss, and no external dependencies or libraries.

## Usage

### Setup

1.  Add `napi-bind` as a dev dependency to your npm or yarn project:

    ```bash
    $ npm install --save-dev napi-bind
    # Or
    $ yarn add --dev napi-bind
    ```

2.  Add the include directories for `napi-bind` to your `binding.gyp` file:

    ```yaml
    {
        'targets': [
            {
                'target_name': 'my_binding_library',
                'include_dirs': [
                    '<!@(node -p "require(\'napi-bind\').include")',
                    # [...]
                ],
                # [...]
            }
            # [...]
        ]
        # [...]
    }
    ```

3.  Add `#include <napi_bind.hpp>` to your C++ headers and begin adding bindings!

### Code

See the [test](./test) directory for some examples of usage.

## Limitations

Note that while this extends to simple C++ functions, `napi-bind` isn't really designed to do more complex wrapping of C++ objects. You might be able to get it to do so with some cleverness, but no guarantees.

If you are looking to create more complex C++ bindings, consider one of the following two libraries:

- https://github.com/nodejs/node-addon-api
- https://github.com/charto/nbind

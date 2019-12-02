{
  'targets': [
    {
      'target_name': 'napi_bind_test',
      'sources': [
        'binding.cpp',
        'containers.cpp',
        'enums.cpp',
        'pointers.cpp',
        'primitives.cpp',
        'strings.cpp',
        'structs.cpp',
      ],
      'defines': [
        'NAPI_EXPERIMENTAL',
        'NAPI_ENABLE_CPP_EXCEPTIONS',
      ],
      'include_dirs': ["<!@(node -p \"require('../').include\")"],
      'cflags': [ '-Werror', '-Wall', '-Wextra', '-Wpedantic', '-Wunused-parameter' ],
      'cflags_cc': [ '-Werror', '-Wall', '-Wextra', '-Wpedantic', '-Wunused-parameter', '-std=c++17' ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        ['OS=="mac"', {
          'cflags+': ['-fvisibility=hidden'],
          'xcode_settings': {
            'OTHER_CFLAGS': ['-fvisibility=hidden']
          }
        }]
      ],
      'msvs_settings': {
        'VCCLCompilerTool': {
          'ExceptionHandling': 1,
          'EnablePREfast': 'true',
        },
      },
      'xcode_settings': {
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.14',
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'OTHER_CFLAGS': [ '-std=c++17'],
      },
    },
  ],
}

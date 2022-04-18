#include <emscripten.h>
#include <emscripten/val.h>

namespace Bridge {
  EM_JS(void, GetRequiredWindowsSize, (int *width, int *height), {
    // clang-format off
    setValue(width, document.body.clientWidth, 'i32');
    setValue(height, document.body.clientHeight, 'i32');
    // clang-format on
  });
}
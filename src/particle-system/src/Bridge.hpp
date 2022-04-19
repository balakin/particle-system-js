#include <emscripten.h>
#include <emscripten/val.h>

namespace Bridge {
  EM_JS(void, GetRequiredWindowsSize, (int *width, int *height), {
    // clang-format off
    const ratio = window.devicePixelRatio;
    setValue(width, Math.floor(document.body.clientWidth * ratio), 'i32');
    setValue(height, Math.floor(document.body.clientHeight * ratio), 'i32');
    // clang-format on
  });
}
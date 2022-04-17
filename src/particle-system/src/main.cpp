#include <emscripten/val.h>

int main() {
  using emscripten::val;
  val document = val::global("document");
  val h1 = document.call<val>("createElement", val("h1"));
  h1.set("innerText", val("Hello World from cpp!"));
  document["body"].call<void>("appendChild", h1);
  return 0;
}

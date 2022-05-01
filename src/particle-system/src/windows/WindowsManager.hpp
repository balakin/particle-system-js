#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <memory>
#include <vector>

#include "Window.hpp"

class WindowsManager : public Window {
private:
  bool toggleButtonPressed = false;
  int savedPosition[2] = {0, 0};
  int savedSize[2] = {0, 0};
  std::vector<std::shared_ptr<Window>> windows;

public:
  WindowsManager();

  void Render() override;
  void AddWindow(Window *window);
};

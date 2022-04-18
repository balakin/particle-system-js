#pragma once

#include "Window.hpp"

class SceneVariables : public Window {
public:
  SceneVariables() : Window("Scene variables") {
  }

  void Render() override;
};

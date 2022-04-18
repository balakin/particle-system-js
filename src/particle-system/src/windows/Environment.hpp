#pragma once

#include "Window.hpp"

class Environment : public Window {
public:
  Environment() : Window("Environment", false) {
  }

  void Render() override;
};

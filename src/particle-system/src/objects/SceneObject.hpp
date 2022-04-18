#pragma once

#include <string>

#include "../math/Transform.hpp"

class SceneObject {
public:
  Transform transform;
  const std::string name;

  SceneObject(const std::string &name, Transform transform) : name(name), transform(transform) {
  }
  SceneObject(const std::string &name) : name(name) {
  }

  virtual void Update(float deltaTime);
  virtual void UI();
  virtual void Render(float deltaTime, float step);
};

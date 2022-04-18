#pragma once

#include <glm/glm.hpp>

class Transform {
private:
  glm::vec3 translation;
  glm::vec3 rotation;
  glm::vec3 scale;
  glm::mat4 matrix;

  void UpdateMatrix();

public:
  Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
      : translation(translation), rotation(rotation), scale(scale) {
    UpdateMatrix();
  }
  Transform() : Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)) {
  }

  void SetTranslation(glm::vec3 translation);
  void SetRotation(glm::vec3 rotation);
  void SetScale(glm::vec3 scale);
  const glm::vec3 &GetTranslation() const;
  const glm::vec3 &GetRotation() const;
  const glm::vec3 &GetScale() const;
  const glm::mat4 &GetMatrix() const;
};

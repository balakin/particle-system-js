#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

void Transform::UpdateMatrix() {
  matrix = glm::mat4(1.0f);
  matrix = glm::translate(matrix, translation);
  matrix = glm::rotate(matrix, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
  matrix = glm::rotate(matrix, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
  matrix = glm::rotate(matrix, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
  matrix = glm::scale(matrix, scale);
}

void Transform::SetTranslation(glm::vec3 translation) {
  this->translation = translation;
  UpdateMatrix();
}

void Transform::SetRotation(glm::vec3 rotation) {
  this->rotation = rotation;
  UpdateMatrix();
}

void Transform::SetScale(glm::vec3 scale) {
  this->scale = scale;
  UpdateMatrix();
}

const glm::vec3 &Transform::GetTranslation() const {
  return translation;
}

const glm::vec3 &Transform::GetRotation() const {
  return rotation;
}

const glm::vec3 &Transform::GetScale() const {
  return scale;
}

const glm::mat4 &Transform::GetMatrix() const {
  return matrix;
}

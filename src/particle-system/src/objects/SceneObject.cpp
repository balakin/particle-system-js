#include "SceneObject.hpp"

#include <glm/glm.hpp>
#include <imgui.h>

void SceneObject::Update(float deltaTime) {
}

void SceneObject::UI() {
  glm::vec3 translation = transform.GetTranslation();
  if (ImGui::DragFloat3("Translation", &translation[0])) {
    transform.SetTranslation(translation);
  }

  glm::vec3 rotation = transform.GetRotation();
  if (ImGui::DragFloat3("Rotation", &rotation[0])) {
    transform.SetRotation(rotation);
  }

  glm::vec3 scale = transform.GetScale();
  if (ImGui::DragFloat3("Scale", &scale[0])) {
    transform.SetScale(scale);
  }
}

void SceneObject::Render(float deltaTime, float step) {
}

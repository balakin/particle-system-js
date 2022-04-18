#include "SceneVariables.hpp"

#include <climits>
#include <imgui.h>

#include "../scenes/Scene.hpp"

void SceneVariables::Render() {
  if (!isOpened) {
    return;
  }

  ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 150.0f), ImVec2(FLT_MAX, FLT_MAX));
  ImGui::Begin(name.c_str(), &isOpened);
  if (!Scene::CurrentExists()) {
    ImGui::Text("No current scene");
    ImGui::End();
    return;
  }

  Scene &scene = Scene::GetCurrent();
  scene.UI();
  ImGui::End();
}

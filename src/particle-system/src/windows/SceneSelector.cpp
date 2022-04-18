#include "SceneSelector.hpp"

#include <imgui.h>

bool SceneSelector::VectorGetter(void *data, int index, const char **outText) {
  auto &vector = *static_cast<std::vector<std::string> *>(data);
  if (index < 0 || index > (int)vector.size()) {
    return false;
  }

  *outText = vector[index].c_str();
  return true;
}

void SceneSelector::Render() {
  if (!isOpened) {
    return;
  }

  ImGui::SetNextWindowSize(ImVec2(280, 130), ImGuiCond_Always);
  ImGui::Begin(name.c_str(), &isOpened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
  if (Scene::CurrentExists()) {
    ImGui::Text("Current scene: %s", Scene::GetCurrentSceneName().c_str());
  } else {
    ImGui::Text("No current scene");
  }
  ImGui::InvisibleButton("##margin", ImVec2(1, 5));
  ImGui::Combo("Scenes", &selectedScene, VectorGetter, &scenes, scenes.size());
  ImGui::InvisibleButton("##margin", ImVec2(1, 5));
  if (ImGui::Button("Select", ImVec2(265, -1))) {
    Scene::Load(scenes[selectedScene]);
  }
  ImGui::End();
}

#include "Scene.hpp"

#include <imgui.h>
#include <stdexcept>

#include "../windows/Performance.hpp"

bool Scene::registrationIsAvailable = true;
bool Scene::changeBlocked;
Scene *Scene::currentScene;
std::string Scene::currentSceneName;
std::map<std::string, std::function<Scene *()>> Scene::sceneConstructors;

bool Scene::SceneObjectGetter(void *data, int index, const char **outText) {
  Scene *scene = static_cast<Scene *>(data);
  if (index < 0 || index > (int)scene->objects.size()) {
    return false;
  }

  *outText = index == 0 ? "Camera" : scene->objects[index - 1]->name.c_str();
  return true;
}

void Scene::BlockRegistration() {
  registrationIsAvailable = false;
}

bool Scene::IsRegistered(const std::string &name) {
  auto it = sceneConstructors.find(name);
  return it != sceneConstructors.end();
}

std::vector<std::string> Scene::GetNamesOfRegistered() {
  std::vector<std::string> registered;
  for (auto const &pair : sceneConstructors) {
    registered.push_back(pair.first);
  }

  return registered;
}

void Scene::Load(const std::string &name) {
  if (changeBlocked) {
    return;
  }

  auto it = sceneConstructors.find(name);
  if (it == sceneConstructors.end()) {
    return;
  }

  CloseCurrent();
  currentScene = it->second();
  currentSceneName = name;
  currentScene->Initialize();
}

Scene &Scene::GetCurrent() {
  if (currentScene == nullptr) {
    throw std::runtime_error("No scene");
  }

  return *currentScene;
}

const std::string &Scene::GetCurrentSceneName() {
  if (currentScene == nullptr) {
    throw std::runtime_error("No scene");
  }

  return currentSceneName;
}

void Scene::CloseCurrent() {
  if (changeBlocked || currentScene == nullptr) {
    return;
  }

  changeBlocked = true;
  currentScene->OnDestroy();
  changeBlocked = false;
  delete currentScene;
  currentScene = nullptr;
  Performance::Data::Reset();
}

bool Scene::CurrentExists() {
  return currentScene != nullptr;
}

Scene::~Scene() {
}

void Scene::SetCamera(const Camera &camera) {
  this->camera = camera;
}

Camera &Scene::GetCamera() {
  return camera;
}

void Scene::Update(float deltaTime) {
  camera.Update(deltaTime);

  for (objectIndex = 0; !isDestroyed && objectIndex < objects.size(); objectIndex++) {
    objects[objectIndex]->Update(deltaTime);
  }
}

void Scene::Render(float deltaTime, float step) {
  for (objectIndex = 0; !isDestroyed && objectIndex < objects.size(); objectIndex++) {
    objects[objectIndex]->Render(deltaTime, step);
  }
}

void Scene::UI() {
  OnUI();
  ImGui::Combo("Objects", &selectedObject, SceneObjectGetter, this, objects.size() + 1);

  if (selectedObject == 0) {
    camera.UI();
  } else {
    objects[selectedObject - 1]->UI();
  }
}

void Scene::OnUI() {
}

void Scene::OnDestroy() {
}

bool Scene::IsDestroyed() const {
  return isDestroyed;
}

void Scene::RemoveObjectAt(size_t index) {
  if (index >= objects.size()) {
    return;
  }

  objects.erase(objects.begin() + index);
  if (objectIndex >= index) {
    objectIndex--;
  }
}

const std::vector<std::shared_ptr<SceneObject>> &Scene::GetObjects() {
  return objects;
}

#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../objects/SceneObject.hpp"
#include "../renderer/Camera.hpp"

class Scene {
private:
  static bool registrationIsAvailable;
  static bool changeBlocked;
  static Scene *currentScene;
  static std::string currentSceneName;
  static std::map<std::string, std::function<Scene *()>> sceneConstructors;

  Camera camera;
  bool isDestroyed = false;
  std::vector<std::shared_ptr<SceneObject>> objects;

  size_t objectIndex = 0;
  int selectedObject = 0;

  static bool SceneObjectGetter(void *data, int index, const char **outText);

public:
  virtual ~Scene();
  template <class T> static void Register(const std::string &name);
  static void BlockRegistration();
  static bool IsRegistered(const std::string &name);
  static std::vector<std::string> GetNamesOfRegistered();
  static void Load(const std::string &name);
  static Scene &GetCurrent();
  static const std::string &GetCurrentSceneName();
  static void CloseCurrent();
  static bool CurrentExists();

  void SetCamera(const Camera &camera);
  Camera &GetCamera();

  virtual void Initialize() = 0;
  void Update(float deltaTime);
  void Render(float deltaTime, float step);
  void UI();
  virtual void OnUI();
  virtual void OnDestroy();
  bool IsDestroyed() const;
  template <class T> std::weak_ptr<T> AddObject(T *obj);
  void RemoveObjectAt(size_t index);
  const std::vector<std::shared_ptr<SceneObject>> &GetObjects();
};

template <class T> void Scene::Register(const std::string &name) {
  static_assert(std::is_base_of<Scene, T>::value);

  if (!registrationIsAvailable || IsRegistered(name)) {
    return;
  }

  sceneConstructors.insert(make_pair(name, []() { return (Scene *)new T(); }));
}

template <class T> inline std::weak_ptr<T> Scene::AddObject(T *object) {
  static_assert(std::is_base_of<SceneObject, T>::value);

  if (object == nullptr) {
    throw new std::runtime_error("no object");
  }

  std::shared_ptr<T> ptr(object);
  objects.push_back(ptr);
  return std::weak_ptr(ptr);
}

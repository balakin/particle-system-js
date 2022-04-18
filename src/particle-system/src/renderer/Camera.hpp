#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "CameraException.hpp"

class Camera {
public:
  enum class Projection
  {
    Orthographic,
    Perspective
  };

  Camera(glm::vec3 translation = glm::vec3(0.0f),
         glm::vec3 rotation = glm::vec3(0.0f),
         glm::vec3 clearColor = glm::vec3(0.0f),
         Camera::Projection projectionType = Camera::Projection::Orthographic)
      : clearColor(clearColor), translation(translation), rotation(rotation) {
    GLFWwindow *window = glfwGetCurrentContext();
    if (!window) {
      throw CameraException("No window");
    }

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    UpdateViewMatrix();
    SetProjection(projectionType, width, height);
  }

  Camera(unsigned int screenWidth,
         unsigned int screenHeight,
         glm::vec3 translation = glm::vec3(0.0f),
         glm::vec3 rotation = glm::vec3(0.0f),
         glm::vec3 clearColor = glm::vec3(0.0f),
         Camera::Projection projectionType = Camera::Projection::Orthographic)
      : clearColor(clearColor), translation(translation), rotation(rotation) {
    UpdateViewMatrix();
    SetProjection(projectionType, screenWidth, screenHeight);
  }

  glm::vec3 clearColor;

  virtual void Update(float deltaTime);
  virtual void UI();

  void SetProjection(Camera::Projection projectionType, unsigned int screenWidth, unsigned int screenHeight);
  void SetProjection(Camera::Projection projectionType);
  Projection GetProjectionType() const;

  void SetScreenSize(unsigned int screenWidth, unsigned int screenHeight);
  glm::ivec2 GetScreenSize() const;

  // Orthographic
  void SetOrhographicProperties(unsigned int size);
  unsigned int GetSize() const;

  // Perspective
  void SetPerspectiveProperties(float fov, float near, float far);
  float GetFOV() const;
  float GetNear() const;
  float GetFar() const;

  void SetTranslation(const glm::vec3 &translation);
  const glm::vec3 &GetTranslation() const;

  void SetRotation(const glm::vec3 &rotation);
  const glm::vec3 &GetRotation() const;

  const glm::mat4 &ProjectionMatrix() const;
  const glm::mat4 &ViewMatrix() const;
  const glm::mat4 &ViewProjectionMatrix() const;

private:
  Projection projectionType;
  glm::mat4 projectionMatrix;
  glm::vec3 translation;
  glm::vec3 rotation;
  glm::mat4 viewMatrix;
  glm::mat4 viewProjectionMatrix;
  unsigned int screenWidth;
  unsigned int screenHeight;

  // Orthographic
  unsigned int size = 5;

  // Perspective
  float fov = 60.0f;
  float near = 0.3f;
  float far = 1000.0f;

  void UpdateProjectionMatrix();
  void UpdateViewMatrix();
  void UpdateViewProjectionMatrix();
};

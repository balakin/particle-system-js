#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

void Camera::UpdateProjectionMatrix() {
  float width = float(screenWidth);
  float height = float(screenHeight);
  float aspectRatio = width / height;
  float zoom = float(size);
  switch (projectionType) {
  case Camera::Projection::Orthographic:
    this->projectionMatrix = glm::ortho(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);
    break;
  case Camera::Projection::Perspective:
    this->projectionMatrix = glm::perspectiveFov(glm::radians(fov), width, height, near, far);
    break;
  default:
    return;
  }
}

void Camera::UpdateViewMatrix() {
  viewMatrix = glm::mat4(1.0f);
  viewMatrix = glm::translate(viewMatrix, translation);
  viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
  viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
  viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
  viewMatrix = glm::inverse(viewMatrix);
}

void Camera::UpdateViewProjectionMatrix() {
  viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::Update(float deltaTime) {
}

void Camera::UI() {
  ImGui::Text("Camera");

  if (ImGui::DragFloat3("Translation", &translation[0])) {
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
  }

  if (ImGui::DragFloat3("Rotation", &rotation[0])) {
    UpdateViewMatrix();
    UpdateViewProjectionMatrix();
  }

  ImGui::ColorEdit3("Clear color", &clearColor[0]);

  bool orthographic = projectionType == Camera::Projection::Orthographic;
  if (ImGui::Checkbox("Orthographic projection", &orthographic)) {
    SetProjection(orthographic ? Camera::Projection::Orthographic : Camera::Projection::Perspective);
  }

  if (orthographic) {
    int size = this->size;
    if (ImGui::SliderInt("Size", &size, 1, 100)) {
      SetOrhographicProperties(size);
    }
  } else {
    float fov = this->fov;
    float near = this->near;
    float far = this->far;
    bool changed = false;
    changed = ImGui::SliderFloat("FOV", &fov, 0.01f, 179.99f, "%.2f");
    changed = ImGui::SliderFloat("Near", &near, 0.01f, 3.0f, "%.2f") || changed;
    changed = ImGui::SliderFloat("Far", &far, near + 1, 1000.0f, "%.2f") || changed;
    if (changed) {
      SetPerspectiveProperties(fov, near, far);
    }
  }
}

void Camera::SetProjection(Camera::Projection projectionType, unsigned int screenWidth, unsigned int screenHeight) {
  if (screenWidth == 0) {
    throw CameraException("Argument \"screenWidth\" must be greater than 0");
  }

  if (screenHeight == 0) {
    throw CameraException("Argument \"screenHeight\" must be greater than 0");
  }

  if (projectionType == this->projectionType && screenWidth == this->screenWidth &&
      screenHeight == this->screenHeight) {
    return;
  }

  this->projectionType = projectionType;
  this->screenWidth = screenWidth;
  this->screenHeight = screenHeight;
  UpdateProjectionMatrix();
  UpdateViewProjectionMatrix();
}

void Camera::SetProjection(Camera::Projection projectionType) {
  if (projectionType != this->projectionType) {
    SetProjection(projectionType, screenWidth, screenHeight);
  }
}

Camera::Projection Camera::GetProjectionType() const {
  return projectionType;
}

void Camera::SetScreenSize(unsigned int screenWidth, unsigned int screenHeight) {
  if (screenWidth != this->screenWidth || screenHeight != this->screenHeight) {
    SetProjection(projectionType, screenWidth, screenHeight);
  }
}

glm::ivec2 Camera::GetScreenSize() const {
  return glm::ivec2(screenWidth, screenHeight);
}

void Camera::SetOrhographicProperties(unsigned int size) {
  if (size == 0) {
    throw CameraException("Argument \"size\" must be greater than 0");
  }

  this->size = size;
  if (projectionType == Camera::Projection::Orthographic) {
    UpdateProjectionMatrix();
    UpdateViewProjectionMatrix();
  }
}

unsigned int Camera::GetSize() const {
  return size;
}

void Camera::SetPerspectiveProperties(float fov, float near, float far) {
  if (fov <= 0.0f || fov >= 180.0f) {
    throw CameraException("Argument \"fov\" must be in range (0; 180)");
  }

  if (near <= 0) {
    throw CameraException("Argument \"near\" must be greater than 0");
  }

  if (far <= near) {
    throw CameraException("Argument \"far\" must be greater than \"near\"");
  }

  this->fov = fov;
  this->near = near;
  this->far = far;
  if (projectionType == Camera::Projection::Perspective) {
    UpdateProjectionMatrix();
    UpdateViewProjectionMatrix();
  }
}

float Camera::GetFOV() const {
  return fov;
}

float Camera::GetNear() const {
  return near;
}

float Camera::GetFar() const {
  return far;
}

void Camera::SetTranslation(const glm::vec3 &translation) {
  this->translation = translation;
  UpdateViewMatrix();
  UpdateViewProjectionMatrix();
}

const glm::vec3 &Camera::GetTranslation() const {
  return translation;
}

void Camera::SetRotation(const glm::vec3 &rotation) {
  this->rotation = rotation;
  UpdateViewMatrix();
  UpdateViewProjectionMatrix();
}

const glm::vec3 &Camera::GetRotation() const {
  return rotation;
}

const glm::mat4 &Camera::ProjectionMatrix() const {
  return projectionMatrix;
}

const glm::mat4 &Camera::ViewMatrix() const {
  return viewMatrix;
}

const glm::mat4 &Camera::ViewProjectionMatrix() const {
  return viewProjectionMatrix;
}

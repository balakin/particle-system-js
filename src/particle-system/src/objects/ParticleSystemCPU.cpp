#include "ParticleSystemCPU.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "../renderer/Renderer.hpp"
#include "ParticleSystemException.hpp"

void ParticleSystemCPU::Compute(float deltaTime) {
  unsigned int activeCount = this->activeCount;
  struct Particle *particle = firstParticle;
  struct Particle *buffer = nullptr;
  for (size_t i = 0; i < activeCount; i++, particle = particle->right) {
    particle->lifeRemaining -= deltaTime;
    if (particle->lifeRemaining <= 0.0f) {
      this->activeCount--;
      if (particle == firstParticle) {
        firstParticle = firstParticle->right;
      } else {
        buffer = particle->left;
        particle->left->right = particle->right;
        particle->right->left = particle->left;
        firstParticle->left->right = particle;
        particle->left = firstParticle->left;
        particle->right = firstParticle;
        firstParticle->left = particle;
        particle = buffer;
      }
    } else {
      particle->previousPosition = particle->position;
      particle->position += particle->velocity * deltaTime;

      particle->previousRotation = particle->rotation;
      particle->rotation += particle->angularVelocity * deltaTime;
      particle->rotation.x -= std::floor(particle->rotation.x / 360.0f) * 360.0f;
      particle->rotation.y -= std::floor(particle->rotation.y / 360.0f) * 360.0f;
      particle->rotation.z -= std::floor(particle->rotation.z / 360.0f) * 360.0f;

      float k = particle->lifeRemaining / particle->lifespan;

      particle->previousSize = particle->size;
      particle->size = glm::mix(particle->sizeEnd, particle->sizeBegin, k);

      particle->previousColor = particle->color;
      particle->color = glm::mix(particle->colorEnd, particle->colorBegin, k);
    }
  }
}

void ParticleSystemCPU::Emit(unsigned int count) {
  count = std::min(count, poolSize - activeCount);
  activeCount += count;
  for (size_t i = 0; i < count; i++) {
    float lifespanVariation = particleProperties.lifespanVariation * (Random() - 0.5f);
    freeParticle->lifespan = particleProperties.lifespan + lifespanVariation;
    freeParticle->lifeRemaining = particleProperties.lifespan + lifespanVariation;

    freeParticle->position = particleProperties.position;
    freeParticle->position.x += particleProperties.positionVariation.x * (Random() - 0.5f);
    freeParticle->position.y += particleProperties.positionVariation.y * (Random() - 0.5f);
    freeParticle->position.z += particleProperties.positionVariation.z * (Random() - 0.5f);
    freeParticle->previousPosition = freeParticle->position;

    freeParticle->rotation = particleProperties.rotation;
    freeParticle->rotation.x += particleProperties.rotationVariation.x * (Random() - 0.5f);
    freeParticle->rotation.y += particleProperties.rotationVariation.y * (Random() - 0.5f);
    freeParticle->rotation.z += particleProperties.rotationVariation.z * (Random() - 0.5f);
    freeParticle->previousRotation = freeParticle->rotation;

    freeParticle->velocity = particleProperties.velocity;
    freeParticle->velocity.x += particleProperties.velocityVariation.x * (Random() - 0.5f);
    freeParticle->velocity.y += particleProperties.velocityVariation.y * (Random() - 0.5f);
    freeParticle->velocity.z += particleProperties.velocityVariation.z * (Random() - 0.5f);

    freeParticle->angularVelocity = particleProperties.angularVelocity;
    freeParticle->angularVelocity.x += particleProperties.angularVelocityVariation.x * (Random() - 0.5f);
    freeParticle->angularVelocity.y += particleProperties.angularVelocityVariation.y * (Random() - 0.5f);
    freeParticle->angularVelocity.z += particleProperties.angularVelocityVariation.z * (Random() - 0.5f);

    freeParticle->color = particleProperties.colorBegin;
    freeParticle->colorBegin = particleProperties.colorBegin;
    freeParticle->colorEnd = particleProperties.colorEnd;
    freeParticle->previousColor = freeParticle->color;

    float sizeVariation = particleProperties.sizeVariation * (Random() - 0.5f);
    freeParticle->sizeBegin = particleProperties.sizeBegin + sizeVariation;
    freeParticle->sizeEnd = particleProperties.sizeEnd + sizeVariation;
    freeParticle->size = freeParticle->sizeBegin;
    freeParticle->previousSize = freeParticle->size;

    freeParticle = freeParticle->right;
  }
}

void ParticleSystemCPU::RenderParticle(const Particle *particle, float k) {
  const glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
  const glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
  const glm::vec3 zAxis(0.0f, 0.0f, 1.0f);

  glm::vec3 position = glm::mix(particle->previousPosition, particle->position, k);
  glm::vec3 rotation = glm::mix(particle->previousRotation, particle->rotation, k);
  glm::vec4 color = glm::mix(particle->previousColor, particle->color, k);
  float size = glm::mix(particle->previousSize, particle->size, k);

  glm::mat4 model = glm::translate(transform.GetMatrix(), position);
  if (rotation.x != 0.0f) {
    model = glm::rotate(model, glm::radians(rotation.x), xAxis);
  }

  if (rotation.y != 0.0f) {
    model = glm::rotate(model, glm::radians(rotation.y), yAxis);
  }

  if (rotation.z != 0.0f) {
    model = glm::rotate(model, glm::radians(rotation.z), zAxis);
  }

  model = glm::scale(model, glm::vec3(size, size, 0.0f));
  Renderer::DrawQuad(model, color);
}

void ParticleSystemCPU::OnRenderEnd() {
}

ParticleSystemCPU::ParticleSystemCPU(const std::string &name,
                                     unsigned int poolSize,
                                     unsigned int particlesPerFrame,
                                     ParticleProperties particleProperties)
    : EmitParticleSystem(name, particlesPerFrame, particleProperties) {
  SetPoolSize(poolSize);
  auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
  randomGenerator = std::mt19937_64(seed);
  distribution = std::uniform_real_distribution<float>(0.0f, 1.0f);
}

float ParticleSystemCPU::Random() {
  return distribution(randomGenerator);
}

void ParticleSystemCPU::SetPoolSize(unsigned int size) {
  if (size == 0) {
    throw ParticleSystemException("Pool size should be greater than 0");
  }

  if (size == poolSize) {
    return;
  }

  if (pool != nullptr) {
    delete[] pool;
  }

  pool = new struct Particle[size];
  for (size_t i = 0; i < size - 1; i++) {
    pool[i].right = &pool[i + 1];
    pool[i + 1].left = &pool[i];
  }

  pool[size - 1].right = &pool[0];
  pool[0].left = &pool[size - 1];

  poolSize = size;
  firstParticle = &pool[0];
  freeParticle = &pool[0];
  activeCount = 0;
}

void ParticleSystemCPU::UI() {
  EmitParticleSystem::UI();
  int changedParticlesPerFrame = particlesPerFrame;
  ImGui::DragInt("Particles per frame", &changedParticlesPerFrame);
  if (changedParticlesPerFrame != particlesPerFrame && changedParticlesPerFrame > 0) {
    particlesPerFrame = changedParticlesPerFrame;
  }

  int changedPoolSize = poolSize;
  ImGui::DragInt("Pool size", &changedPoolSize);
  if (changedPoolSize != poolSize && changedPoolSize > 0) {
    SetPoolSize(changedPoolSize);
  }

  ImGui::Text("Particles per frame: %d", particlesPerFrame);
  ImGui::Text("Pool size: %d", poolSize);
  ImGui::Text("Active particles: %d", activeCount);
  ImGui::Text("Free: %d", poolSize - activeCount);
}

void ParticleSystemCPU::Render(float deltaTime, float step) {
  struct Particle *particle = firstParticle;
  const float k = deltaTime / step;
  for (size_t i = 0; i < activeCount; i++, particle = particle->right) {
    RenderParticle(particle, k);
  }

  OnRenderEnd();
}

#pragma once

#include <glm/glm.hpp>
#include <random>
#include <string>

#include "EmitParticleSystem.hpp"

class ParticleSystemCPU : public EmitParticleSystem {
protected:
  struct Particle {
    float lifespan;
    float lifeRemaining;
    glm::vec3 position;
    glm::vec3 previousPosition;
    glm::vec3 velocity;
    glm::vec3 rotation;
    glm::vec3 previousRotation;
    glm::vec3 angularVelocity;
    glm::vec4 color;
    glm::vec4 previousColor;
    glm::vec4 colorBegin;
    glm::vec4 colorEnd;
    float size;
    float previousSize;
    float sizeBegin;
    float sizeEnd;

    struct Particle *left;
    struct Particle *right;
  };

  void Compute(float deltaTime) override;
  void Emit(unsigned int count) override;
  virtual void RenderParticle(const Particle *particle, float k);
  virtual void OnRenderEnd();

private:
  struct Particle *pool = nullptr;
  struct Particle *firstParticle = nullptr;
  struct Particle *freeParticle = nullptr;
  unsigned int poolSize = 0;
  unsigned int activeCount = 0;
  std::mt19937_64 randomGenerator;
  std::uniform_real_distribution<float> distribution;

  float Random();

public:
  ParticleSystemCPU(const std::string &name,
                    unsigned int poolSize,
                    unsigned int particlesPerFrame,
                    ParticleProperties particleProperties);

  void SetPoolSize(unsigned int size);

  void UI() override;
  void Render(float deltaTime, float step) final;
};

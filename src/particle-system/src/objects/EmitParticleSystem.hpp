#pragma once

#include <glm/glm.hpp>
#include <string>

#include "SceneObject.hpp"

class EmitParticleSystem : public SceneObject {
protected:
  virtual void Compute(float deltaTime) = 0;
  virtual void Emit(unsigned int count) = 0;

public:
  struct ParticleProperties {
    float lifespan;
    float lifespanVariation;
    glm::vec3 position;
    glm::vec3 positionVariation;
    glm::vec3 velocity;
    glm::vec3 velocityVariation;
    glm::vec3 rotation;
    glm::vec3 rotationVariation;
    glm::vec3 angularVelocity;
    glm::vec3 angularVelocityVariation;
    glm::vec4 colorBegin;
    glm::vec4 colorEnd;
    float sizeBegin;
    float sizeEnd;
    float sizeVariation;
  };

  ParticleProperties particleProperties;
  unsigned int particlesPerFrame;

  EmitParticleSystem(const std::string &name, unsigned int particlesPerFrame, ParticleProperties particleProperties);

  void Update(float deltaTime) final;
  void UI() override;
};

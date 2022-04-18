#include "ParticleSystemCPUandGPU.hpp"

#include <GL/glew.h>

#include "../renderer/Camera.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererError.hpp"
#include "../scenes/Scene.hpp"
#include "../utils/Resources.hpp"

ParticleSystemCPUandGPU::Data::Data(size_t quadCount) {
  maxVertexCount = quadCount;

  std::vector<BufferElement> bufferElements;
  bufferElements.push_back(BufferElement(BufferElement::Type::Float3, "VertexPosition", false));
  bufferElements.push_back(BufferElement(BufferElement::Type::Float3, "VertexRotation", false));
  bufferElements.push_back(BufferElement(BufferElement::Type::Float4, "VertexColor", false));
  bufferElements.push_back(BufferElement(BufferElement::Type::Float, "VertexSize", false));
  BufferLayout layout(bufferElements);

  stride = layout.GetStride();
  vertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(stride * maxVertexCount, layout));
  vertexArray.AddVertexBuffer(*vertexBuffer);
  vertices = new float[stride * maxVertexCount];
}

ParticleSystemCPUandGPU::Data::~Data() {
  delete[] vertices;
}

void ParticleSystemCPUandGPU::Data::SetPosition(const glm::vec3 &position) {
  vertices[offset++] = position[0];
  vertices[offset++] = position[1];
  vertices[offset++] = position[2];
}

void ParticleSystemCPUandGPU::Data::SetRotation(const glm::vec3 &rotation) {
  vertices[offset++] = rotation[0];
  vertices[offset++] = rotation[1];
  vertices[offset++] = rotation[2];
}

void ParticleSystemCPUandGPU::Data::SetColor(const glm::vec4 &color) {
  vertices[offset++] = color[0];
  vertices[offset++] = color[1];
  vertices[offset++] = color[2];
  vertices[offset++] = color[3];
}

void ParticleSystemCPUandGPU::Data::SetSize(float size) {
  vertices[offset++] = size;
}

void ParticleSystemCPUandGPU::Data::NextVertex() {
  vertexIndex++;
}

bool ParticleSystemCPUandGPU::Data::CanDraw() {
  return vertexIndex != 0;
}

bool ParticleSystemCPUandGPU::Data::IsFull() {
  return vertexIndex == maxVertexCount;
}

void ParticleSystemCPUandGPU::Data::Draw() {
  if (!CanDraw()) {
    return;
  }

  vertexArray.Bind();
  vertexBuffer->SetData(vertices, vertexIndex * stride);
  glDrawArrays(GL_POINTS, 0, vertexIndex);
}

void ParticleSystemCPUandGPU::Data::Reset() {
  vertexIndex = 0;
  offset = 0;
}

void ParticleSystemCPUandGPU::Draw() {
  if (!data->CanDraw() || !Scene::CurrentExists()) {
    return;
  }

  shader.Use();
  Camera &camera = Scene::GetCurrent().GetCamera();
  shader.SetUniform("ModelMatrix", transform.GetMatrix());
  shader.SetUniform("ViewProjectionMatrix", camera.ViewProjectionMatrix());
  Renderer::Draw(data);
}

ParticleSystemCPUandGPU::ParticleSystemCPUandGPU(const std::string &name,
                                                 unsigned int poolSize,
                                                 unsigned int particlesPerFrame,
                                                 ParticleProperties particleProperties,
                                                 unsigned int maxQuadInBatch)
    : ParticleSystemCPU(name, poolSize, particlesPerFrame, particleProperties) {
  data = nullptr;
  try {
    shader.Add(GL_VERTEX_SHADER, Resources::LoadText("assets/shaders/CPUandGPU.vert.glsl"));
    shader.Add(GL_GEOMETRY_SHADER, Resources::LoadText("assets/shaders/CPUandGPU.geom.glsl"));
    shader.Add(GL_FRAGMENT_SHADER, Resources::LoadText("assets/shaders/Color.frag.glsl"));
    shader.Link();
  } catch (std::runtime_error error) {
    throw RendererError("Failed to create shader: " + std::string(error.what()));
  }

  data = new Data(maxQuadInBatch);
}

ParticleSystemCPUandGPU::~ParticleSystemCPUandGPU() {
  if (data != nullptr) {
    delete data;
  }
}

void ParticleSystemCPUandGPU::RenderParticle(const ParticleSystemCPU::Particle *particle, float k) {
  glm::vec3 position = glm::mix(particle->previousPosition, particle->position, k);
  glm::vec3 rotation = glm::mix(particle->previousRotation, particle->rotation, k);
  glm::vec4 color = glm::mix(particle->previousColor, particle->color, k);
  float size = glm::mix(particle->previousSize, particle->size, k);

  if (data->IsFull()) {
    Draw();
  }

  data->SetPosition(position);
  data->SetRotation(rotation);
  data->SetColor(color);
  data->SetSize(size);
  data->NextVertex();
}

void ParticleSystemCPUandGPU::OnRenderEnd() {
  Draw();
}

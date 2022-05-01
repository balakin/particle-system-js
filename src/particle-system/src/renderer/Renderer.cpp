#include "Renderer.hpp"

#include <GL/glew.h>
#include <chrono>
#include <stdexcept>
#include <vector>

#include "../scenes/Scene.hpp"
#include "../utils/Resources.hpp"
#include "BufferElement.hpp"
#include "BufferLayout.hpp"
#include "RendererError.hpp"

const glm::vec4 Renderer::Data::positions[4] = {glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
                                                glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f),
                                                glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
                                                glm::vec4(0.5f, -0.5f, 0.0f, 1.0f)};

Renderer::Data::Data(unsigned int quadCount) {
  maxQuadCount = quadCount;
  maxVertexCount = quadCount * 4;
  maxIndexCount = quadCount * 6;

  std::vector<BufferElement> bufferElements;
  bufferElements.push_back(BufferElement(BufferElement::Type::Float3, "VertexPosition", false));
  bufferElements.push_back(BufferElement(BufferElement::Type::Float4, "VertexColor", false));
  BufferLayout layout(bufferElements);

  stride = layout.GetStride();
  vertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(stride * maxVertexCount, layout));
  vertexArray.AddVertexBuffer(*vertexBuffer);
  vertices = new float[stride * maxVertexCount];

  unsigned short *indices = new unsigned short[maxIndexCount];
  unsigned short offset = 0;
  for (unsigned int i = 0; i < maxIndexCount; i += 6, offset += 4) {
    indices[i + 0] = offset + 0;
    indices[i + 1] = offset + 1;
    indices[i + 2] = offset + 2;

    indices[i + 3] = offset + 0;
    indices[i + 4] = offset + 3;
    indices[i + 5] = offset + 2;
  }

  indexBuffer = std::unique_ptr<IndexBuffer>(new IndexBuffer(indices, maxIndexCount));
  vertexArray.SetIndexBuffer(*indexBuffer);
  vertexArray.Unbind();
  vertexBuffer->Unbind();
  indexBuffer->Unbind();
  delete[] indices;

  Reset();
}

Renderer::Data::~Data() {
  delete[] vertices;
}

void Renderer::Data::SetPosition(const glm::mat4 &modelMatrix) {
  glm::vec4 position = modelMatrix * positions[vertexIndex];
  vertices[offset++] = position[0];
  vertices[offset++] = position[1];
  vertices[offset++] = position[2];
}

void Renderer::Data::SetColor(const glm::vec4 &color) {
  vertices[offset++] = color[0];
  vertices[offset++] = color[1];
  vertices[offset++] = color[2];
  vertices[offset++] = color[3];
}

void Renderer::Data::NextVertex() {
  vertexIndex++;
  if (vertexIndex == 4) {
    vertexIndex = 0;
    quadIndex++;
  }
}

bool Renderer::Data::CanDraw() {
  return quadIndex != 0;
}

bool Renderer::Data::IsFull() {
  return quadIndex == maxQuadCount;
}

void Renderer::Data::Draw() {
  if (!CanDraw()) {
    return;
  }

  vertexArray.Bind();
  vertexBuffer->SetData(vertices, quadIndex * stride * 4);
  glDrawElements(GL_TRIANGLES, (GLsizei)quadIndex * 6, GL_UNSIGNED_SHORT, nullptr);
  vertexArray.Unbind();
}

void Renderer::Data::Reset() {
  quadIndex = 0;
  vertexIndex = 0;
  offset = 0;
}

Renderer *Renderer::renderer = nullptr;

Renderer::Renderer(unsigned int maxQuadCount) {
  if (renderer != nullptr) {
    throw RendererError("Renderer already exists");
  }

  if (maxQuadCount > quadCountLimit) {
    throw RendererError("The limit of the number of vertices is exceeded");
  }

  try {
    shader.Add(GL_VERTEX_SHADER, Resources::LoadText("assets/shaders/Color.vert.glsl"));
    shader.Add(GL_FRAGMENT_SHADER, Resources::LoadText("assets/shaders/Color.frag.glsl"));
    shader.Link();
  } catch (std::runtime_error error) {
    throw RendererError("Failed to create shader: " + std::string(error.what()));
  }

  renderer = this;
  Setup(maxQuadCount);
}

Renderer::~Renderer() {
  delete data;
  renderer = nullptr;
}

void Renderer::BeginScene(SDL_Window *window) {
  if (isScene) {
    throw RendererError("Scene has already begun");
  }

  isScene = true;
  statistics.drawCalls = 0;
  statistics.renderTime = std::chrono::milliseconds(0);
  if (!Scene::CurrentExists()) {
    return;
  }

  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  glViewport(0, 0, width, height);

  Camera &camera = Scene::GetCurrent().GetCamera();
  camera.SetScreenSize(width, height);
  viewProjectionMatrix = camera.ViewProjectionMatrix();
  const glm::vec3 &clearColor = camera.clearColor;
  glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndScene() {
  Flush();
  isScene = false;
}

void Renderer::Flush() {
  if (!isScene) {
    return;
  }

  shader.Use();
  shader.SetUniform("ViewProjectionMatrix", viewProjectionMatrix);
  Renderer::Draw(data);
  shader.Disable();
}

void Renderer::Setup(unsigned int maxQuadCount) {
  if (renderer == nullptr) {
    throw RendererError("No renderer");
  }

  if (maxQuadCount > quadCountLimit) {
    throw RendererError("The limit of the number of vertices is exceeded");
  }

  if (renderer->data != nullptr) {
    if (renderer->isScene) {
      renderer->Flush();
    }

    delete renderer->data;
  }

  renderer->data = new Renderer::Data(maxQuadCount);
}

const Renderer::Statistics &Renderer::GetStatistics() {
  if (renderer == nullptr) {
    throw RendererError("No renderer");
  }

  return renderer->statistics;
}

void Renderer::DrawQuad(const glm::mat4 modelMatrix, const glm::vec4 &color) {
  if (renderer == nullptr) {
    throw RendererError("No renderer");
  }

  if (!renderer->isScene) {
    return;
  }

  if (renderer->data->IsFull()) {
    renderer->Flush();
  }

  for (unsigned int i = 0; i < 4; i++) {
    renderer->data->SetPosition(modelMatrix);
    renderer->data->SetColor(color);
    renderer->data->NextVertex();
  }
}

void Renderer::Draw(DrawData *data) {
  if (!data->CanDraw()) {
    return;
  }

  if (!Scene::CurrentExists()) {
    data->Reset();
    return;
  }

  auto start = std::chrono::high_resolution_clock::now();

  data->Draw();

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
  renderer->statistics.drawCalls++;
  renderer->statistics.renderTime += duration;

  data->Reset();
}

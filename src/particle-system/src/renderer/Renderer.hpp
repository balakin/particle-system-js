#pragma once

#include <chrono>
#include <glm/glm.hpp>
#include <memory>

#include "DrawData.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

class Renderer {
private:
  class Data : public DrawData {
  private:
    float *vertices;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;
    VertexArray vertexArray;

    unsigned int maxQuadCount;
    unsigned int maxVertexCount;
    unsigned int maxIndexCount;
    unsigned int stride;

    unsigned int quadIndex = 0;
    unsigned int vertexIndex = 0;
    unsigned int offset = 0;

    static const glm::vec4 positions[4];

  public:
    Data(unsigned int quadCount);
    ~Data();

    void SetPosition(const glm::mat4 &modelMatrix);
    void SetColor(const glm::vec4 &color);
    void NextVertex();

    bool CanDraw();
    bool IsFull();

    void Draw();
    void Reset();
  };

  struct Statistics {
    unsigned int drawCalls = 0;
    std::chrono::nanoseconds renderTime;
  };

  Renderer::Data *data = nullptr;
  bool isScene = false;
  Shader shader;
  glm::mat4 viewProjectionMatrix;
  Statistics statistics;

  void Flush();

  static Renderer *renderer;

public:
  static const unsigned int quadCountLimit = 25000;

  Renderer(unsigned int maxQuadCount);
  ~Renderer();

  void BeginScene();
  void EndScene();

  static void Setup(unsigned int maxQuadCount);

  static const Statistics &GetStatistics();

  static void DrawQuad(const glm::mat4 modelMatrix, const glm::vec4 &color);

  static void Draw(DrawData *data);
};

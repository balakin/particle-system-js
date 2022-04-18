#pragma once

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"

class VertexArray {
private:
  unsigned int vao;
  unsigned int attribArrayIndex = 0;

  unsigned int BufferElementTypeToOpenGLType(BufferElement::Type type);

public:
  VertexArray();
  ~VertexArray();

  void Bind() const;
  void Unbind() const;

  void AddVertexBuffer(const VertexBuffer &buffer);
  void SetIndexBuffer(const IndexBuffer &buffer);
};

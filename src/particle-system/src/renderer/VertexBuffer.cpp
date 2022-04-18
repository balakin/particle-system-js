#include "VertexBuffer.hpp"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(void *data, unsigned int size, const BufferLayout &layout) : layout(layout) {
  glGenBuffers(1, &buffer);
  Bind();
  glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(unsigned int size, const BufferLayout &layout) : layout(layout) {
  glGenBuffers(1, &buffer);
  Bind();
  glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &buffer);
}

void VertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

void VertexBuffer::Unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(void *data, unsigned int size) {
  Bind();
  glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)size, data);
}

const BufferLayout &VertexBuffer::GetLayout() const {
  return layout;
}

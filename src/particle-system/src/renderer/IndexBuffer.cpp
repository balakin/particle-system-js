#include "IndexBuffer.hpp"

#include <GL/glew.h>

IndexBuffer::IndexBuffer(unsigned short *indices, unsigned int size) : size(size) {
  glGenBuffers(1, &buffer);
  Bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(size * sizeof(unsigned short)), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &buffer);
}

void IndexBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}

void IndexBuffer::Unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::GetSize() const {
  return size;
}

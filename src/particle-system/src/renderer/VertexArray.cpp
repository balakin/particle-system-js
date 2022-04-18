#include "VertexArray.hpp"

#include <GL/glew.h>
#include <stdexcept>

#include "BufferElement.hpp"
#include "BufferLayout.hpp"

unsigned int VertexArray::BufferElementTypeToOpenGLType(BufferElement::Type type) {
  switch (type) {
  case BufferElement::Type::Bool:
    return GL_BOOL;
  case BufferElement::Type::Int:
  case BufferElement::Type::Int2:
  case BufferElement::Type::Int3:
  case BufferElement::Type::Int4:
    return GL_INT;
  case BufferElement::Type::Float:
  case BufferElement::Type::Float2:
  case BufferElement::Type::Float3:
  case BufferElement::Type::Float4:
  case BufferElement::Type::Mat3:
  case BufferElement::Type::Mat4:
    return GL_FLOAT;
  default:
    throw std::runtime_error("Unknown element type");
  }
}

VertexArray::VertexArray() {
  glGenVertexArrays(1, &vao);
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &vao);
}

void VertexArray::Bind() const {
  glBindVertexArray(vao);
}

void VertexArray::Unbind() const {
  glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const VertexBuffer &buffer) {
  Bind();
  buffer.Bind();

  const BufferLayout &layout = buffer.GetLayout();

  unsigned int count;
  for (const BufferElement &element : layout.GetElements()) {
    switch (element.type) {
    case BufferElement::Type::Bool:
    case BufferElement::Type::Int:
    case BufferElement::Type::Int2:
    case BufferElement::Type::Int3:
    case BufferElement::Type::Int4:
      glEnableVertexAttribArray(attribArrayIndex);
      glVertexAttribIPointer(attribArrayIndex,
                             element.GetComponentCount(),
                             BufferElementTypeToOpenGLType(element.type),
                             layout.GetStride(),
                             (const void *)element.offset);
      attribArrayIndex++;
      break;
    case BufferElement::Type::Float:
    case BufferElement::Type::Float2:
    case BufferElement::Type::Float3:
    case BufferElement::Type::Float4:
      glEnableVertexAttribArray(attribArrayIndex);
      glVertexAttribPointer(attribArrayIndex,
                            element.GetComponentCount(),
                            BufferElementTypeToOpenGLType(element.type),
                            element.normalized,
                            layout.GetStride(),
                            (const void *)element.offset);
      attribArrayIndex++;
      break;
    case BufferElement::Type::Mat3:
    case BufferElement::Type::Mat4:
      count = element.GetComponentCount();
      for (unsigned int i = 0; i < count; i++) {
        glEnableVertexAttribArray(attribArrayIndex);
        glVertexAttribPointer(attribArrayIndex,
                              count,
                              BufferElementTypeToOpenGLType(element.type),
                              element.normalized,
                              layout.GetStride(),
                              (const void *)(element.offset + sizeof(float) * count * i));
      }
      break;
    default:
      throw std::runtime_error("Unknown element type");
    }
  }
}

void VertexArray::SetIndexBuffer(const IndexBuffer &buffer) {
  Bind();
  buffer.Bind();
}

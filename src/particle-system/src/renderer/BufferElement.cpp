#include "BufferElement.hpp"

#include <stdexcept>

unsigned int BufferElement::TypeToSize(BufferElement::Type type) {
  switch (type) {
  case BufferElement::Type::Bool:
    return sizeof(bool);
  case BufferElement::Type::Int:
    return sizeof(int);
  case BufferElement::Type::Int2:
    return sizeof(int) * 2;
  case BufferElement::Type::Int3:
    return sizeof(int) * 3;
  case BufferElement::Type::Int4:
    return sizeof(int) * 4;
  case BufferElement::Type::Float:
    return sizeof(float);
  case BufferElement::Type::Float2:
    return sizeof(float) * 2;
  case BufferElement::Type::Float3:
    return sizeof(float) * 3;
  case BufferElement::Type::Float4:
    return sizeof(float) * 4;
  case BufferElement::Type::Mat3:
    return sizeof(float) * 3 * 3;
  case BufferElement::Type::Mat4:
    return sizeof(float) * 4 * 4;
  default:
    throw std::runtime_error("Unknown element type");
  }
}

unsigned int BufferElement::GetComponentCount() const {
  switch (type) {
  case BufferElement::Type::Bool:
    return 1;
  case BufferElement::Type::Int:
    return 1;
  case BufferElement::Type::Int2:
    return 2;
  case BufferElement::Type::Int3:
    return 3;
  case BufferElement::Type::Int4:
    return 4;
  case BufferElement::Type::Float:
    return 1;
  case BufferElement::Type::Float2:
    return 2;
  case BufferElement::Type::Float3:
    return 3;
  case BufferElement::Type::Float4:
    return 4;
  case BufferElement::Type::Mat3:
    return 3;
  case BufferElement::Type::Mat4:
    return 4;
  default:
    throw std::runtime_error("Unknown element type");
  }
}

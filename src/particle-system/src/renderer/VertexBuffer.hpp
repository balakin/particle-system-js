#pragma once

#include "BufferLayout.hpp"

class VertexBuffer {
private:
  unsigned int buffer;
  BufferLayout layout;

public:
  VertexBuffer(void *data, unsigned int size, const BufferLayout &layout);
  VertexBuffer(unsigned int size, const BufferLayout &layout);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;

  void SetData(void *data, unsigned int size);

  const BufferLayout &GetLayout() const;
};

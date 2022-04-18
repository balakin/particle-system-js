#pragma once

class IndexBuffer {
private:
  unsigned int buffer;
  unsigned int size;

public:
  IndexBuffer(unsigned short *indices, unsigned int size);
  ~IndexBuffer();

  void Bind() const;
  void Unbind() const;

  unsigned int GetSize() const;
};

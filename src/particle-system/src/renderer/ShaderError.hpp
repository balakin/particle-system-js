#pragma once

#include <stdexcept>

class ShaderError : public std::runtime_error {
public:
  ShaderError(const std::string &message) : std::runtime_error(message) {
  }
};

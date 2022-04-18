#pragma once

#include <stdexcept>
#include <string>

class RendererError : public std::runtime_error {
public:
  RendererError(const std::string &message) : std::runtime_error(message) {
  }
};

#pragma once

#include <stdexcept>
#include <string>

class CameraException : public std::runtime_error {
public:
  CameraException(const std::string &message) : std::runtime_error(message) {
  }
};

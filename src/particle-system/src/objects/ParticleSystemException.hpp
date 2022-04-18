#pragma once

#include <stdexcept>
#include <string>

class ParticleSystemException : public std::runtime_error {
public:
  ParticleSystemException(std::string message) : std::runtime_error(message) {
  }
};

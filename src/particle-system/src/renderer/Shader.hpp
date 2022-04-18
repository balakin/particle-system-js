#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Shader {
private:
  bool vertexShaderAttached = false;
  bool fragmentShaderAttached = false;
  bool linked = false;
  unsigned int program;
  std::unordered_map<std::string, unsigned int> uniformLocations;

  bool IsValidShaderType(unsigned int shaderType);
  bool ShaderAttached(unsigned int shaderType);
  int GetUniformLocation(const std::string &name);

  static std::string ShaderTypeToString(unsigned int shaderType);

public:
  Shader();
  ~Shader();

  void Add(unsigned int shaderType, const std::string &source);
  void Link();
  void Use();
  void Disable();

  void SetUniform(const std::string &name, float x, float y, float z);
  void SetUniform(const std::string &name, const glm::vec2 &vector);
  void SetUniform(const std::string &name, const glm::vec3 &vector);
  void SetUniform(const std::string &name, const glm::vec4 &vector);
  void SetUniform(const std::string &name, const glm::mat3 &matrix);
  void SetUniform(const std::string &name, const glm::mat4 &matrix);
  void SetUniform(const std::string &name, unsigned int value);
  void SetUniform(const std::string &name, float value);
  void SetUniform(const std::string &name, bool value);
  void SetUniform(const std::string &name, int value);
};

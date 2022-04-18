#include "Shader.hpp"

#include <GL/glew.h>

#include "ShaderError.hpp"

bool Shader::IsValidShaderType(unsigned int shaderType) {
  switch (shaderType) {
  case GL_VERTEX_SHADER:
  case GL_FRAGMENT_SHADER:
    return true;
  default:
    return false;
  }
}

bool Shader::ShaderAttached(unsigned int shaderType) {
  switch (shaderType) {
  case GL_VERTEX_SHADER:
    return vertexShaderAttached;
  case GL_FRAGMENT_SHADER:
    return fragmentShaderAttached;
  default:
    return false;
  }
}

int Shader::GetUniformLocation(const std::string &name) {
  auto it = uniformLocations.find(name);

  if (it == uniformLocations.end()) {
    int location = glGetUniformLocation(program, name.c_str());
    if (location < 0) {
      throw new ShaderError("No uniform with name - " + name);
    }

    return uniformLocations[name] = location;
  }

  return uniformLocations[name];
}

std::string Shader::ShaderTypeToString(unsigned int shaderType) {
  switch (shaderType) {
  case GL_VERTEX_SHADER:
    return "Vertex";
  case GL_FRAGMENT_SHADER:
    return "Fragment";
  default:
    return "Unknown";
  }
}

Shader::Shader() {
  program = glCreateProgram();
  if (!program) {
    throw new ShaderError("Cannot create shader program");
  }
}

Shader::~Shader() {
  glDeleteProgram(program);
}

void Shader::Add(unsigned int shaderType, const std::string &source) {
  if (linked) {
    return;
  }

  if (!IsValidShaderType(shaderType)) {
    throw ShaderError("Unknown shader type");
  }

  if (ShaderAttached(shaderType)) {
    throw ShaderError(ShaderTypeToString(shaderType) + " shader already attached");
  }

  unsigned int shader = glCreateShader(shaderType);
  if (!shader) {
    throw ShaderError("Cannot create shader");
  }

  const char *code = source.c_str();
  glShaderSource(shader, 1, &code, nullptr);

  glCompileShader(shader);

  int result;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (!result) {
    int logLength;
    std::string log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
      char *cLog = new char[logLength];
      int written;
      glGetShaderInfoLog(shader, logLength, &written, cLog);
      log = cLog;
      delete[] cLog;
    }

    throw ShaderError("Compilation failed: " + log);
  }

  switch (shaderType) {
  case GL_VERTEX_SHADER:
    vertexShaderAttached = true;
    break;
  case GL_FRAGMENT_SHADER:
    fragmentShaderAttached = true;
    break;
  }

  glAttachShader(program, shader);
  glDeleteShader(shader);
}

void Shader::Link() {
  if (linked) {
    return;
  }

  glLinkProgram(program);

  int status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    int logLength;
    std::string log;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
      char *cLog = new char[logLength];
      int written;
      glGetProgramInfoLog(program, logLength, &written, cLog);
      log = cLog;
      delete[] cLog;
    }

    throw ShaderError("Link failed: " + log);
  }

  linked = true;
}

void Shader::Use() {
  if (!linked) {
    return;
  }

  glUseProgram(program);
}

void Shader::Disable() {
  glUseProgram(0);
}

void Shader::SetUniform(const std::string &name, float x, float y, float z) {
  int location = GetUniformLocation(name);
  glUniform3f(location, x, y, z);
}

void Shader::SetUniform(const std::string &name, const glm::vec2 &vector) {
  int location = GetUniformLocation(name);
  glUniform2f(location, vector.x, vector.y);
}

void Shader::SetUniform(const std::string &name, const glm::vec3 &vector) {

  int location = GetUniformLocation(name);
  glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::SetUniform(const std::string &name, const glm::vec4 &vector) {

  int location = GetUniformLocation(name);
  glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetUniform(const std::string &name, const glm::mat3 &matrix) {

  int location = GetUniformLocation(name);
  glUniformMatrix3fv(location, 1, false, &matrix[0][0]);
}

void Shader::SetUniform(const std::string &name, const glm::mat4 &matrix) {
  int location = GetUniformLocation(name);
  glUniformMatrix4fv(location, 1, false, &matrix[0][0]);
}

void Shader::SetUniform(const std::string &name, unsigned int value) {
  int location = GetUniformLocation(name);
  glUniform1ui(location, value);
}

void Shader::SetUniform(const std::string &name, float value) {
  int location = GetUniformLocation(name);
  glUniform1f(location, value);
}

void Shader::SetUniform(const std::string &name, bool value) {
  int location = GetUniformLocation(name);
  glUniform1i(location, value);
}

void Shader::SetUniform(const std::string &name, int value) {
  int location = GetUniformLocation(name);
  glUniform1i(location, value);
}

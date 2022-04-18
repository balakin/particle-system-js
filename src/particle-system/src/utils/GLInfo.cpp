#include "GLInfo.hpp"

#include <GL/glew.h>

bool GLInfo::isInitialized;
const unsigned char *GLInfo::vendor;
const unsigned char *GLInfo::renderer;
const unsigned char *GLInfo::version;
const unsigned char *GLInfo::glslVersion;
int GLInfo::major;
int GLInfo::minor;
glm::ivec3 GLInfo::maxComputeWorkGroupCount;
glm::ivec3 GLInfo::maxComputeWorkGroupSize;
int GLInfo::maxComputeWorkGroupInvocations;

void GLInfo::Initialize() {
  if (isInitialized) {
    return;
  }

  isInitialized = true;
  vendor = glGetString(GL_VENDOR);
  renderer = glGetString(GL_RENDERER);
  version = glGetString(GL_VERSION);
  glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);

  for (unsigned int i = 0; i < 3; i++) {
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, i, &maxComputeWorkGroupCount[i]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, i, &maxComputeWorkGroupSize[i]);
  }

  glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &maxComputeWorkGroupInvocations);
}

const unsigned char *GLInfo::Vendor() {
  Initialize();
  return vendor;
}

const unsigned char *GLInfo::Renderer() {
  Initialize();
  return renderer;
}

const unsigned char *GLInfo::Version() {
  Initialize();
  return version;
}

const unsigned char *GLInfo::GLSLVersion() {
  Initialize();
  return glslVersion;
}

int GLInfo::Major() {
  Initialize();
  return major;
}

int GLInfo::Minor() {
  Initialize();
  return minor;
}

const glm::ivec3 &GLInfo::MaxComputeWorkGroupCount() {
  Initialize();
  return maxComputeWorkGroupCount;
}

const glm::ivec3 &GLInfo::MaxComputeWorkGroupSize() {
  Initialize();
  return maxComputeWorkGroupSize;
}

int GLInfo::MaxComputeWorkGroupInvocations() {
  Initialize();
  return maxComputeWorkGroupInvocations;
}

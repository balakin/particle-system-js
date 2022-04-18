#pragma once

#include <glm/glm.hpp>

class GLInfo {
private:
  static bool isInitialized;
  static const unsigned char *vendor;
  static const unsigned char *renderer;
  static const unsigned char *version;
  static const unsigned char *glslVersion;
  static int major;
  static int minor;
  static glm::ivec3 maxComputeWorkGroupCount;
  static glm::ivec3 maxComputeWorkGroupSize;
  static int maxComputeWorkGroupInvocations;

  static void Initialize();

public:
  static const unsigned char *Vendor();
  static const unsigned char *Renderer();
  static const unsigned char *Version();
  static const unsigned char *GLSLVersion();
  static int Major();
  static int Minor();
  static const glm::ivec3 &MaxComputeWorkGroupCount();
  static const glm::ivec3 &MaxComputeWorkGroupSize();
  static int MaxComputeWorkGroupInvocations();
};

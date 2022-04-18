#include "Environment.hpp"

#include <imgui.h>

#include "../utils/GLInfo.hpp"

void Environment::Render() {
  if (!isOpened) {
    return;
  }

  ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
  ImGui::Begin(name.c_str(), &isOpened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
  ImGui::Text("GL Vendor: %s", GLInfo::Vendor());
  ImGui::Text("GL Renderer: %s", GLInfo::Renderer());
  ImGui::Text("GL Version: %s", GLInfo::Version());
  ImGui::Text("GLSL Version: %s", GLInfo::GLSLVersion());
  ImGui::InvisibleButton("##margin", ImVec2(1, 5));
  ImGui::Text("Max compute work group count: (%d, %d, %d)",
              GLInfo::MaxComputeWorkGroupCount().x,
              GLInfo::MaxComputeWorkGroupCount().y,
              GLInfo::MaxComputeWorkGroupCount().z);
  ImGui::Text("Max compute work group size: (%d, %d, %d)",
              GLInfo::MaxComputeWorkGroupSize().x,
              GLInfo::MaxComputeWorkGroupSize().y,
              GLInfo::MaxComputeWorkGroupSize().z);
  ImGui::Text("Max compute work group invocations: %d", GLInfo::MaxComputeWorkGroupInvocations());
  ImGui::End();
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Bridge.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/RendererError.hpp"
#include "scenes/Sandbox.hpp"
#include "scenes/Scene.hpp"
#include "scenes/cpu/EasyTestCPU.hpp"
#include "scenes/cpu/HardTestCPU.hpp"
#include "scenes/cpu/NormalTestCPU.hpp"
#include "windows/Environment.hpp"
#include "windows/Performance.hpp"
#include "windows/SceneSelector.hpp"
#include "windows/SceneVariables.hpp"
#include "windows/WindowsManager.hpp"

void RegisterScenes() {
  Scene::Register<Sandbox>("Sandbox");

  Scene::Register<EasyTestCPU>("Easy test (CPU)");
  Scene::Register<NormalTestCPU>("Normal test (CPU)");
  Scene::Register<HardTestCPU>("Hard test (CPU)");

  Scene::BlockRegistration();
}

void ErrorCallbackGLFW(int errorCode, const char *description) {
  emscripten_console_errorf("GLFW (%d): %s", errorCode, description);
}

bool InitGLFW() {
#if _DEBUG
  glfwSetErrorCallback(ErrorCallbackGLFW);
#endif
  if (!glfwInit()) {
    emscripten_console_error("(GLFW) failed to initialize");
    return false;
  }

  return true;
}

GLFWwindow *CreateGLFWWindow(const char *title) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  int width, height;
  Bridge::GetRequiredWindowsSize(&width, &height);
  GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window) {
    emscripten_console_error("(GLFW) failed to create window");
    return nullptr;
  }

  glfwMakeContextCurrent(window);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  return window;
}

bool InitGLEW() {
  unsigned int code = glewInit();
  if (code != GLEW_OK) {
    emscripten_console_errorf("GLEW: %s", glewGetErrorString(code));
    emscripten_console_error("(GLEW) failed to initialize");
  }

  return true;
}

ImGuiIO &InitImGui(GLFWwindow *window) {
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 18.0f);
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  // Dark theme. Thanks Cherno. https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/ImGui/ImGuiLayer.cpp

  auto &colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

  // Headers
  colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Buttons
  colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Frame BG
  colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
  colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
  colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

  // Title
  colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  return io;
}

void ImGuiNewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void RenderImGuiFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void InitWindowsManager(WindowsManager &windowsManager) {
  windowsManager.AddWindow(new SceneSelector());
  windowsManager.AddWindow(new SceneVariables());
  windowsManager.AddWindow(new Environment());
  windowsManager.AddWindow(new Performance());
}

std::function<void()> loop;

void Loop() {
  loop();
}

int main() {
  RegisterScenes();

  if (!InitGLFW()) {
    return EXIT_FAILURE;
  }

  GLFWwindow *window = CreateGLFWWindow("Particle system");
  if (!window) {
    return EXIT_FAILURE;
  }

  if (!InitGLEW()) {
    return EXIT_FAILURE;
  }

  InitImGui(window);

  WindowsManager windowsManager(window);
  InitWindowsManager(windowsManager);

  Renderer *renderer = nullptr;
  try {
    renderer = new Renderer(5000);
  } catch (RendererError error) {
    emscripten_console_error("Shader error occured in main");
    emscripten_console_error(error.what());
    emscripten_console_error("Cannot create renderer");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  Performance::Data *performanceData = new Performance::Data();

  unsigned int fps = 60;
  double lastFrameTime = 0.0;
  double deltaTime = 0.0;
  double step = 1.0 / fps;
  loop = [&] {
    double currentFrameTime = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();

    std::chrono::milliseconds updateTime;
    std::chrono::nanoseconds renderTimeCPU;
    std::chrono::nanoseconds renderTimeGPU;
    bool dataCollected = false;

    int width, height;
    Bridge::GetRequiredWindowsSize(&width, &height);
    glfwSetWindowSize(window, width, height);
    if (Scene::CurrentExists() && width != 0 && height != 0) {
      Scene &scene = Scene::GetCurrent();
      deltaTime += std::min(0.1, currentFrameTime - lastFrameTime);

      auto start = std::chrono::high_resolution_clock::now();
      while (deltaTime >= step) {
        deltaTime -= step;
        scene.Update(float(step));
        if (scene.IsDestroyed()) {
          break;
        }
      }

      auto stop = std::chrono::high_resolution_clock::now();
      updateTime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

      if (!scene.IsDestroyed()) {
        start = std::chrono::high_resolution_clock::now();
        renderer->BeginScene();
        scene.Render(float(deltaTime), float(step));
        renderer->EndScene();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        renderTimeGPU = renderer->GetStatistics().renderTime;
        renderTimeCPU = duration - renderTimeGPU;

        if (!scene.IsDestroyed()) {
          dataCollected = true;
        }
      }
    }

    ImGuiNewFrame();
    windowsManager.Render();
    RenderImGuiFrame();

    if (Scene::CurrentExists() && dataCollected) {
      performanceData->Add(updateTime,
                           std::chrono::duration_cast<std::chrono::milliseconds>(renderTimeCPU),
                           std::chrono::duration_cast<std::chrono::milliseconds>(renderTimeGPU),
                           1.0 / (currentFrameTime - lastFrameTime));
    }

    glfwSwapBuffers(window);
    lastFrameTime = currentFrameTime;
  };

  emscripten_set_main_loop(Loop, 0, true);

  delete renderer;
  Scene::CloseCurrent();
  glfwTerminate();
  return EXIT_SUCCESS;
}

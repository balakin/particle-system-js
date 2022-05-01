#include <GL/glew.h>
#include <SDL.h>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

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

bool InitSDL() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    emscripten_console_errorf("(SDL) failed to initialize: %s", SDL_GetError());
    return false;
  }

  return true;
}

SDL_Window *CreateSDLWindow(const char *title) {
  int width, height;
  Bridge::GetRequiredWindowsSize(&width, &height);
  SDL_Window *window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
  if (!window) {
    emscripten_console_errorf("(SDL) failed to create window: %s", SDL_GetError());
    return nullptr;
  }

  return window;
}

SDL_GLContext CreateGLContext(SDL_Window *window) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  return context;
}

bool InitGLEW() {
  unsigned int code = glewInit();
  if (code != GLEW_OK) {
    emscripten_console_errorf("(GLEW) failed to initialize: %s", glewGetErrorString(code));
  }

  return true;
}

ImGuiIO &InitImGui(SDL_Window *window, SDL_GLContext context) {
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 18.0f);
  ImGui_ImplSDL2_InitForOpenGL(window, context);
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
  ImGui_ImplSDL2_NewFrame();
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

  if (!InitSDL()) {
    return EXIT_FAILURE;
  }

  SDL_Window *window = CreateSDLWindow("Particle system");
  if (!window) {
    return EXIT_FAILURE;
  }

  SDL_GLContext context = CreateGLContext(window);
  if (!InitGLEW()) {
    return EXIT_FAILURE;
  }

  InitImGui(window, context);

  WindowsManager windowsManager;
  InitWindowsManager(windowsManager);

  Renderer *renderer = nullptr;
  try {
    renderer = new Renderer(5000);
  } catch (RendererError error) {
    emscripten_console_error("Shader error occured in main");
    emscripten_console_error(error.what());
    emscripten_console_error("Cannot create renderer");
    return EXIT_FAILURE;
  }

  Performance::Data *performanceData = new Performance::Data();

  unsigned int fps = 60;
  double lastFrameTime = 0.0;
  double deltaTime = 0.0;
  double step = 1.0 / fps;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  SDL_Event event;
  loop = [&] {
    double currentFrameTime = (double)SDL_GetTicks64() / 1000.0;
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) {
        emscripten_cancel_main_loop();
        return;
      }
    }

    std::chrono::milliseconds updateTime;
    std::chrono::nanoseconds renderTimeCPU;
    std::chrono::nanoseconds renderTimeGPU;
    bool dataCollected = false;

    int width, height;
    Bridge::GetRequiredWindowsSize(&width, &height);
    SDL_SetWindowSize(window, width, height);
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
        renderer->BeginScene(window);
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

    SDL_GL_SwapWindow(window);
    lastFrameTime = currentFrameTime;
  };

  emscripten_set_main_loop(Loop, 0, true);

  delete renderer;
  Scene::CloseCurrent();
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}

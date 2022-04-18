#include "Performance.hpp"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <imgui.h>
#include <stdexcept>

#include "../scenes/Scene.hpp"

Performance::Data *Performance::Data::data = nullptr;

void Performance::Data::ThrowIfNoData() {
  if (data == nullptr) {
    throw std::runtime_error("No data");
  }
}

Performance::Data::Data() {
  if (Performance::Data::data != nullptr) {
    throw std::runtime_error("Data already exists");
  }

  Performance::Data::data = this;
}

Performance::Data::~Data() {
  Performance::Data::data = nullptr;
}

void Performance::Data::Add(const std::chrono::milliseconds &updateTime,
                            const std::chrono::milliseconds &renderTimeCPU,
                            const std::chrono::milliseconds &renderTimeGPU,
                            double fps) {
  if (stopped) {
    return;
  }

  iteration++;

  long long valueUpdateTime = updateTime.count();
  long long valueRenderTimeCPU = renderTimeCPU.count();
  long long valueRenderTimeGPU = renderTimeGPU.count();

  this->fps = fps;
  frameTime = valueUpdateTime + valueRenderTimeCPU + valueRenderTimeGPU;
  this->updateTime = valueUpdateTime;
  this->renderTimeCPU = valueRenderTimeCPU;
  this->renderTimeGPU = valueRenderTimeGPU;

  minFPS = std::min(minFPS, fps);
  minFrameTime = std::min(minFrameTime, frameTime);
  minUpdateTime = std::min(minUpdateTime, valueUpdateTime);
  minRenderTimeCPU = std::min(minRenderTimeCPU, valueRenderTimeCPU);
  minRenderTimeGPU = std::min(minRenderTimeGPU, valueRenderTimeGPU);

  maxFPS = std::max(maxFPS, fps);
  maxFrameTime = std::max(maxFrameTime, frameTime);
  maxUpdateTime = std::max(maxUpdateTime, valueUpdateTime);
  maxRenderTimeCPU = std::max(maxRenderTimeCPU, valueRenderTimeCPU);
  maxRenderTimeGPU = std::max(maxRenderTimeGPU, valueRenderTimeGPU);

  totalFPS += fps;
  totalFrameTime += frameTime;
  totalUpdateTime += valueUpdateTime;
  totalRenderTimeCPU += valueRenderTimeCPU;
  totalRenderTimeGPU += valueRenderTimeGPU;
}

void Performance::Data::Resume() {
  ThrowIfNoData();
  data->stopped = false;
}

void Performance::Data::Stop() {
  ThrowIfNoData();
  data->stopped = true;
}

bool Performance::Data::IsStopped() {
  ThrowIfNoData();
  return data->stopped;
}

void Performance::Data::Reset() {
  if (data == nullptr) {
    return;
  }

  data->stopped = false;

  data->iteration = 1;

  data->fps = 0.0;
  data->frameTime = 0;
  data->updateTime = 0;
  data->renderTimeCPU = 0;
  data->renderTimeGPU = 0;

  data->minFPS = DBL_MAX;
  data->minFrameTime = LLONG_MAX;
  data->minUpdateTime = LLONG_MAX;
  data->minRenderTimeCPU = LLONG_MAX;
  data->minRenderTimeGPU = LLONG_MAX;

  data->maxFPS = 0;
  data->maxFrameTime = 0;
  data->maxUpdateTime = 0;
  data->maxRenderTimeCPU = 0;
  data->maxRenderTimeGPU = 0;

  data->totalFPS = 0;
  data->totalFrameTime = 0;
  data->totalUpdateTime = 0;
  data->totalRenderTimeCPU = 0;
  data->totalRenderTimeGPU = 0;
}

double Performance::Data::GetFPS() {
  ThrowIfNoData();
  return data->fps;
}

long long Performance::Data::GetFrameTime() {
  ThrowIfNoData();
  return data->frameTime;
}

long long Performance::Data::GetUpdateTime() {
  ThrowIfNoData();
  return data->updateTime;
}

long long Performance::Data::GetRenderTimeCPU() {
  ThrowIfNoData();
  return data->renderTimeCPU;
}

long long Performance::Data::GetRenderTimeGPU() {
  ThrowIfNoData();
  return data->renderTimeGPU;
}

double Performance::Data::GetMinFPS() {
  ThrowIfNoData();
  return data->minFPS;
}

long long Performance::Data::GetMinFrameTime() {
  ThrowIfNoData();
  return data->minFrameTime;
}

long long Performance::Data::GetMinUpdateTime() {
  ThrowIfNoData();
  return data->minUpdateTime;
}

long long Performance::Data::GetMinRenderTimeCPU() {
  ThrowIfNoData();
  return data->minRenderTimeCPU;
}

long long Performance::Data::GetMinRenderTimeGPU() {
  ThrowIfNoData();
  return data->minRenderTimeGPU;
}

double Performance::Data::GetMaxFPS() {
  ThrowIfNoData();
  return data->maxFPS;
}

long long Performance::Data::GetMaxFrameTime() {
  ThrowIfNoData();
  return data->maxFrameTime;
}

long long Performance::Data::GetMaxUpdateTime() {
  ThrowIfNoData();
  return data->maxUpdateTime;
}

long long Performance::Data::GetMaxRenderTimeCPU() {
  ThrowIfNoData();
  return data->maxRenderTimeCPU;
}

long long Performance::Data::GetMaxRenderTimeGPU() {
  ThrowIfNoData();
  return data->maxRenderTimeGPU;
}

double Performance::Data::GetTotalFPS() {
  ThrowIfNoData();
  return data->totalFPS;
}

long long Performance::Data::GetTotalFrameTime() {
  ThrowIfNoData();
  return data->totalFrameTime;
}

long long Performance::Data::GetTotalUpdateTime() {
  ThrowIfNoData();
  return data->totalUpdateTime;
}

long long Performance::Data::GetTotalRenderTimeCPU() {
  ThrowIfNoData();
  return data->totalRenderTimeCPU;
}

long long Performance::Data::GetTotalRenderTimeGPU() {
  ThrowIfNoData();
  return data->totalRenderTimeGPU;
}

double Performance::Data::GetAverageFPS() {
  ThrowIfNoData();
  return data->totalFPS / data->iteration;
}

double Performance::Data::GetAverageFrameTime() {
  ThrowIfNoData();
  return double(data->totalFrameTime) / data->iteration;
}

double Performance::Data::GetAverageUpdateTime() {
  ThrowIfNoData();
  return double(data->totalUpdateTime) / data->iteration;
}

double Performance::Data::GetAverageRenderTimeCPU() {
  ThrowIfNoData();
  return double(data->totalRenderTimeCPU) / data->iteration;
}

double Performance::Data::GetAverageRenderTimeGPU() {
  ThrowIfNoData();
  return double(data->totalRenderTimeGPU) / data->iteration;
}

long long Performance::Data::GetRenderTime() {
  ThrowIfNoData();
  return data->renderTimeCPU + data->renderTimeGPU;
}

Performance::Performance() : Window("Performance", false) {
}

void Performance::Render() {
  if (!isOpened) {
    return;
  }

  ImGui::Begin(name.c_str(), &isOpened);
  if (!Scene::CurrentExists()) {
    ImGui::Text("No data");
    ImGui::End();
    return;
  }

  ImGui::Text("FPS: %lld (min: %lld; avg: %lld; max: %lld)",
              std::llround(Performance::Data::GetFPS()),
              std::llround(Performance::Data::GetMinFPS()),
              std::llround(Performance::Data::GetAverageFPS()),
              std::llround(Performance::Data::GetMaxFPS()));
  ImGui::Text("Frame time: %lld (min: %lld, avg: %lld; max: %lld)",
              Performance::Data::GetFrameTime(),
              Performance::Data::GetMinFrameTime(),
              std::llround(Performance::Data::GetAverageFrameTime()),
              Performance::Data::GetMaxFrameTime());
  ImGui::Text("Update time (CPU): %lld (min: %lld, avg: %lld; max: %lld)",
              Performance::Data::GetUpdateTime(),
              Performance::Data::GetMinUpdateTime(),
              std::llround(Performance::Data::GetAverageUpdateTime()),
              Performance::Data::GetMaxUpdateTime());
  ImGui::Text("Render time (CPU): %lld (min: %lld, avg: %lld; max: %lld)",
              Performance::Data::GetRenderTimeCPU(),
              Performance::Data::GetMinRenderTimeCPU(),
              std::llround(Performance::Data::GetAverageRenderTimeCPU()),
              Performance::Data::GetMaxRenderTimeCPU());
  ImGui::Text("Render time (GPU): %lld (min: %lld, avg: %lld; max: %lld)",
              Performance::Data::GetRenderTimeGPU(),
              Performance::Data::GetMinRenderTimeGPU(),
              std::llround(Performance::Data::GetAverageRenderTimeGPU()),
              Performance::Data::GetMaxRenderTimeGPU());
  if (ImGui::Button("Reset")) {
    Performance::Data::Reset();
  }

  ImGui::SameLine();
  if (Performance::Data::IsStopped()) {
    if (ImGui::Button("Resume")) {
      Performance::Data::Resume();
    }
  } else {
    if (ImGui::Button("Stop")) {
      Performance::Data::Stop();
    }
  }

  ImGui::End();
}

#pragma once

#include <cfloat>
#include <chrono>
#include <climits>

#include "Window.hpp"

class Performance : public Window {
public:
  class Data {
  private:
    bool stopped = false;

    long long iteration = 1;

    double fps = 0.0;
    long long frameTime = 0;
    long long updateTime = 0;
    long long renderTimeCPU = 0;
    long long renderTimeGPU = 0;

    double minFPS = DBL_MAX;
    long long minFrameTime = LLONG_MAX;
    long long minUpdateTime = LLONG_MAX;
    long long minRenderTimeCPU = LLONG_MAX;
    long long minRenderTimeGPU = LLONG_MAX;

    double maxFPS = 0;
    long long maxFrameTime = 0;
    long long maxUpdateTime = 0;
    long long maxRenderTimeCPU = 0;
    long long maxRenderTimeGPU = 0;

    double totalFPS = 0;
    long long totalFrameTime = 0;
    long long totalUpdateTime = 0;
    long long totalRenderTimeCPU = 0;
    long long totalRenderTimeGPU = 0;

    static Performance::Data *data;

    static void ThrowIfNoData();

  public:
    Data();
    ~Data();

    void Add(const std::chrono::milliseconds &updateTime,
             const std::chrono::milliseconds &renderTimeCPU,
             const std::chrono::milliseconds &renderTimeGPU,
             double fps);

    static void Resume();
    static void Stop();

    static bool IsStopped();

    static void Reset();

    static double GetFPS();
    static long long GetFrameTime();
    static long long GetUpdateTime();
    static long long GetRenderTimeCPU();
    static long long GetRenderTimeGPU();

    static double GetMinFPS();
    static long long GetMinFrameTime();
    static long long GetMinUpdateTime();
    static long long GetMinRenderTimeCPU();
    static long long GetMinRenderTimeGPU();

    static double GetMaxFPS();
    static long long GetMaxFrameTime();
    static long long GetMaxUpdateTime();
    static long long GetMaxRenderTimeCPU();
    static long long GetMaxRenderTimeGPU();

    static double GetTotalFPS();
    static long long GetTotalFrameTime();
    static long long GetTotalUpdateTime();
    static long long GetTotalRenderTimeCPU();
    static long long GetTotalRenderTimeGPU();

    static double GetAverageFPS();
    static double GetAverageFrameTime();
    static double GetAverageUpdateTime();
    static double GetAverageRenderTimeCPU();
    static double GetAverageRenderTimeGPU();

    static long long GetRenderTime();
  };

  Performance();

  void Render() override;
};

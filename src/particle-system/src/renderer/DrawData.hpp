#pragma once

class DrawData {
public:
  virtual bool CanDraw() = 0;
  virtual void Draw() = 0;
  virtual void Reset() = 0;
};

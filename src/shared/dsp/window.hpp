#pragma once

namespace dsp {

class Window {
public:
  Window(int width) : width(width) {}

  virtual float operator()(int sampleIndex) = 0;

protected:
  int width;
};

class HannWindow : public Window {
public:
  HannWindow(int width) : Window(width) {}

  float operator()(int sampleIndex);
};

class HammingWindow : public Window {
public:
  HammingWindow(int width) : Window(width) {}

  float operator()(int sampleIndex);
};

class NuttallWindow : public Window {
public:
  NuttallWindow(int width) : Window(width) {}

  float operator()(int sampleIndex);
};

}; // namespace dsp

#pragma once

#include "shared.hpp"

#include "bar.hpp"
#include "scale.hpp"

class LevelMeterComponent : public LayoutComponent {
public:
  LevelMeterComponent(PluginProcessor &);

  void resized() override;

private:
  BarComponent leftBar;
  ScaleComponent scale;
  BarComponent rightBar;
};
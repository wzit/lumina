#pragma once

#include <vector>

namespace lumina {


class TextureUnits {
public:
  static void init();
  static bool isPrimed(int index);
  static int countPrimed();
  static void setPrimed(int index);
  static void unsetPrimed(int index);

private:
  static std::vector<bool> s_primedUnits;
  static int s_primedCount;
};

int getMaxTexUnits();

}

#include "TextureUnits.tpp"
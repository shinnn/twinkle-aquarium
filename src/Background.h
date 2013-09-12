#pragma once

#include "ofMain.h"

class Background {
public:
  static void draw(int w, int h);
  
  static bool isMonochrome;

  static int x;
  static int y;
};

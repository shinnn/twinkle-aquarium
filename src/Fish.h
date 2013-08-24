#pragma once

#include "ofMain.h"
#include "Boid3d.h"

class Fish {
public:
  Fish();
  Fish(ofImage * ofI);
  
  ofImage * img;
  
  void setImage(ofImage * ofI);
  void draw(Boid3d * b);
  void drawImage(Boid3d * b);
  
private:
  int imgWidth;
  int imgHeight;
};

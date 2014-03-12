#pragma once

#include "ofMain.h"
#include "Flock3d.h"

class Shoal {
public:
  Shoal(ofImage ofI);

  static float defaultScale;
  static float defaultSpeed;
  static float defaultAlign;

  static float defaultMaxBoundZ;
  static float defaultMinBoundZ;

  static float defaultEscapeSpeed;
  static float maxForce;

  static float additional_vx;
  static float additional_vy;
  static float additional_vz;
  
  static bool isHidden;

  void setupBoids(int num, int _w, int _h);
  void update(double force);
  
  Flock3d flock;
  
  float speed;
  float align;

  float minBoundZ;
  float maxBoundZ;
  
  bool turningZ;
  
  float scale;
  int alpha;
  int saturation;
  int brightness;

  ofImage img;
  int imgWidth;
  int imgHeight;

  void drawOnBoid(Boid3d * b);
};

// bubbles in the sea
class Bubble: public Shoal {

public:
  Bubble(ofImage ofI);

};

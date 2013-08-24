#pragma once

#include "ofMain.h"
#include "Flock3d.h"
#include "ofxSoundInput.h"
#include "Util.h"
#include "Fish.h"
#include "Background.h"

class testApp_D : public ofBaseApp{
public:
  
  static const int FISH_NUM[];

  static const int FPS = 60;
  
  static const float MAX_FORCE;
  static const float MAX_ALIGN;
  static const float MIN_SPEED;
  
  void setup();
  void update();
  void draw();
  void exit();

  void windowResized(int w, int h);
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  
  bool paused;
  bool devMode;
  bool imgMode;
  int cursorHidingFrameCount;

  float winWidth;
  float winHeight;

  ofFbo fbo;
  
  Flock3d flocks[3];
  double force;
  float stream;

  ofxSoundInput soundInput;
  float volume;
  list <float> formarVolumes;
  list <float> currentVolumes;
  float volumeBase;
  
  Util util;

  vector< vector<Fish> > fish;
  ofImage * fishImages;
};


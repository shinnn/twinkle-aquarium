#pragma once

#include "ofMain.h"
#include "Shoal.h"
#include "Util.h"
#include "Background.h"
#include "ofxXmlSettings.h"
#include "ofxSoundInput.h"

class App : public ofBaseApp {

public:
  static int FISH_NUM[];
  static const int FPS;
  static const float MAX_FORCE;
  
  // main methods
  void setup();
  void update();
  void draw();
  
  // other methods
  void setAllBounds();
  
  // Eventlistener
  void windowResized(int w, int h);
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void exit();
  
  // status
  bool paused;
  bool isFullscreen;
  bool devMode;
  bool imgMode;
  bool showTrack;
  unsigned int cursorHidingFrameCount;
  
  bool coordinatesVisible;
  
  // basic background color
  float bgColorR;
  float bgColorG;
  float bgColorB;
  
  float attractionPointScale;
  
  // window size
  float winWidth;
  float winHeight;

  ofFbo fbo;
  
  vector <Shoal> shoals;
  double force;
  
  ofxSoundInput soundInput;

  float volume;
  float maxVolume;
  
  vector <float> formarVolumes;
  vector <float> currentVolumes;
  float volumeBase;
  
  ofCamera camera;
  
  Util util;
  
  ofxXmlSettings settings;
};

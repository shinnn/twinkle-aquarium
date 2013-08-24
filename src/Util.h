#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"

class Util
{
public:
  Util();
  
  void setTitle(bool devMode);
  
  float updateListAverage(list<float> * l, float v);
  
  void makeUnmadeDir(string dirpath);
  
  void updateScreenCaptureParams();
  void saveScreenShot();
  void toggleScreenRecording(int w, int h, int fps);
  void stopRecordScreen();

  ofImage screenImage;
  ofRectangle windowRect;
  ofxVideoRecorder vidRecorder;
  bool isRecording;

};

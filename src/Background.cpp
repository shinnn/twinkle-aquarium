#include "Background.h"

void Background::draw(){
  // 水中背景の描画
  
  // For文用
  int i;

  ofSetLineWidth(2.0);
  
  // HSBモードのColor用
  int hue = 140;
  int sat = 200;
  int bright = 250;
  int alpha = 0;
  
  // RGBモードのColor用
  int R = 30;
  int G = 60;
  int B = 234;
  int A = 0;
  
  // ofEnableBlendMode(OF_BLENDMODE_ADD);
  
  // HSBモードで色線を描く
  for(i=0; i<ofGetHeight(); i++){
    ofColor HSBc;
    HSBc.setHsb(hue, sat+(i/5) , bright+(i/3) , alpha+(i/5) );
    ofSetColor(HSBc);
    ofLine(0, i, ofGetWidth(), i);
  }
  
  // RGBモードで色線を描く
  for(i=0; i < ofGetHeight(); i++){
    ofColor RGBc;
    RGBc.set(R+(i/5), G+(i/5), B-(i/5), A+(i/5));
    ofSetColor(RGBc);
    ofLine(0, i+100, ofGetWidth(), i+100);
  }
}

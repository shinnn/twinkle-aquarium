#include "Util.h"

Util::Util(){
  isRecording = false;
  vidRecorder.setVideoCodec("mpeg4");
  vidRecorder.setVideoBitrate("48000k");
}

void Util::setTitle(bool devMode){
  stringstream strstrm;
  
  // アプリケーションを開いてからの経過時間の表示
  if(devMode){
    float seconds = ofGetElapsedTimef();
    string _h = ofToString(((int) seconds /60) % 60);
    if(_h.length() < 2){
      _h = "0" + _h;
    }
    
    string _s = ofToString((int) seconds % 60);
    if(_s.length() < 2){
      _s = "0" + _s;
    }
    strstrm << "TIME: " << _h << ":" << _s << " ";
  }
  
  // フレームレートの表示
  strstrm << "FPS: " << ofToString(ofGetFrameRate(), 4);
  
  ofSetWindowTitle(strstrm.str());
}

float Util::updateListAverage(list<float> * l, float v){
  l->push_back(v);
  l->pop_front();
  
  float _sum = 0.0f;
  list<float>::iterator itr = l->begin();
  list<float>::iterator itrEnd = l->end();
  
  for(; itr != itrEnd; itr++){
		_sum += *itr;
  }
  
  return 1.0 / l->size() * _sum;
}

float Util::updateVectorAverage(vector<float> * l, float v){
  l->push_back(v);
  l->erase(l->begin());
  
  float _sum = 0.0f;
  vector<float>::iterator itr = l->begin();
  vector<float>::iterator itrEnd = l->end();
  
  for(; itr != itrEnd; itr++){
		_sum += *itr;
  }
  
  return 1.0 / l->size() * _sum;
}

// 対象のフォルダが存在しない場合、そのフォルダを作成する
void Util::makeUnmadeDir(string dirpath){
  if(! ofDirectory(dirpath).exists()){
    ofDirectory::createDirectory(dirpath);
  }
}

void Util::updateScreenCaptureParams(){
  if(!isRecording){
    return;
  }
  windowRect = ofGetWindowRect();
  screenImage.grabScreen(windowRect.x, windowRect.y, windowRect.width, windowRect.height);
  vidRecorder.addFrame(screenImage.getPixelsRef());
}

void Util::saveScreenShot(){
  makeUnmadeDir("screen_shot");
  
  windowRect = ofGetWindowRect();
  if(!isRecording){
    screenImage.grabScreen(windowRect.x, windowRect.y, windowRect.width, windowRect.height);
  }
  screenImage.saveImage("screen_shot/" + ofGetTimestampString() + ".tiff");
}

void Util::toggleScreenRecording(int w, int h, int fps){
  isRecording = !vidRecorder.isInitialized();
  
  if(isRecording){
    makeUnmadeDir("screen_record");
    vidRecorder.setup("screen_record/" + ofGetTimestampString() + ".mov", w, h, fps);
  }else{
    stopRecordScreen();
  }
}

void Util::stopRecordScreen(){
  vidRecorder.close();
}

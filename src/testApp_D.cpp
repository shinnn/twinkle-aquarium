#include "testApp_D.h"

int testApp_D::FISH_NUM[] = {600, 80, 5, 500};
const int testApp_D::FPS = 60;
const float testApp_D::MAX_FORCE = 20000.0f;

//--------------------------------------------------------------
void testApp_D::setup(){
  
  soundInput.setup();
  
  settings.loadFile("settings.xml");
  
  FISH_NUM[0] = settings.getValue("Fish:num0", 600);
  FISH_NUM[1] = settings.getValue("Fish:num1", 80);
  FISH_NUM[2] = settings.getValue("Fish:num2", 5);
  FISH_NUM[3] = settings.getValue("Fish:num3", 500);

  Shoal::defaultMinBoundZ = settings.getValue("Fish:defaultMinBoundZ", 0);
  Shoal::defaultMaxBoundZ = settings.getValue("Fish:defaultMaxBoundZ", 600);

  ofSetFrameRate(FPS);
  ofBackground(0, 0, 0);
  ofSetCircleResolution(60);
  ofSetVerticalSync(true);
  ofEnableSmoothing();
  ofEnableAlphaBlending();
  
  paused = false;
  isFullscreen = false;
  devMode = false;
  imgMode = true;
  showTrack = false;
  cursorHidingFrameCount = 0;
  ofHideCursor();
  
  Shoal::defaultSpeed *=  60.0 / FPS;

  ofSetBackgroundAuto(!showTrack);

  winWidth = ofGetWidth();
  winHeight = ofGetHeight();
  
  bgColorR = bgColorG = bgColorB = 0;
  
  force = 10000.0f;
  
  shoals.push_back(Shoal(ofImage("images/fish0.png")));
  shoals.push_back(Shoal(ofImage("images/fish1.png")));
  shoals.push_back(Shoal(ofImage("images/jellyfish.png")));
  shoals.push_back(Float(ofImage("images/dust.png")));
  
  // Initialize shoals
  for(int i=0; i < shoals.size(); i++){
    shoals[i].setupBoids(FISH_NUM[i], winWidth, winHeight);
  }

  // ウィンドウサイズに依存する設定の初期化
  windowResized(ofGetWidth(), ofGetHeight());
  
  fbo.allocate(winWidth, winHeight);

  fbo.begin();
  {
    ofClear(255);
    Background::draw(winWidth, winHeight);
  }
  fbo.end();
  
  volume = 0;
  maxVolume = 1.0;

  for(int i=0; i < FPS * 10; i++){
    formarVolumes.push_back(0);
  }
  
  for(int i=0; i < FPS * 0.05; i++){
    currentVolumes.push_back(0);
  }
  
  volumeBase = 0;
  
  maxVolume = settings.getValue("maxVolume", 1.0);
  bgColorR = settings.getValue("bgColorR", 0);
  bgColorG = settings.getValue("bgColorG", 0);
  bgColorB = settings.getValue("bgColorB", 0);
  
  Shoal::defaultScale = settings.getValue("Fish:defaultScale", 1.0f);
  Shoal::defaultAlign = settings.getValue("Fish:defaultAlign", 12.0f);
  Shoal::defaultEscapeSpeed = settings.getValue("Fish:defaultEscapeSpeed", 85.0f);
  
  Shoal::additional_vx = settings.getValue("Fish:additional_vx", 0.04250f);
  Shoal::additional_vy = settings.getValue("Fish:additional_vy", 0.04250f);
  Shoal::additional_vz = settings.getValue("Fish:additional_vz", 0.850f);

  attractionPointScale = settings.getValue("attractionPointScale", 1.0f);

  for(int i=0; i < 250; i++){
    testApp_D::update();
  }
}

//--------------------------------------------------------------
void testApp_D::windowResized(int w, int h){
  winWidth = w;
  winHeight = h;
  
  setAllBounds();
}

//--------------------------------------------------------------
void testApp_D::update(){
  
  // 5秒間経過した場合、マウスカーソルを非表示にする
  if(cursorHidingFrameCount == 5 * FPS){
    ofHideCursor();
    cursorHidingFrameCount = 0;
  }else{
    cursorHidingFrameCount++;
  }
  
  // Get audio volume
  // soundInput.getVolumeLog() returns 0.0 〜 1.0
  volume = MAX(soundInput.getVolumeLog() + volumeBase, 0);
  volume = ofMap(volume, 0, maxVolume, 0, 1, true);

  float formerVolumeAverage = util.updateVectorAverage(& formarVolumes, volume);
  float currentVolumeAverage = util.updateVectorAverage(& currentVolumes, volume);
  
  for(int i=0; i < shoals.size(); i++){
    // acceleration
    if(currentVolumeAverage > formerVolumeAverage || currentVolumeAverage > 0.10f){
      force = MAX_FORCE * currentVolumeAverage;
      shoals[i].flock.setMaxSpeed(shoals[i].speed + Shoal::defaultEscapeSpeed * (force / MAX_FORCE));
    }else{
      if(force < 0.00010f){
        force = 0;
      }else{
        force = force * 0.950f;
      }
      // deceleration
      shoals[i].flock.setMaxSpeed(MAX(shoals[i].flock.getMaxSpeed() * 0.950f, shoals[i].speed));
    }
    
    // 散開するときだけ、魚の整列を乱す
    shoals[i].flock.setAlign(shoals[i].align / (1.0 + volume));
    shoals[i].flock.setDistAlign(90.0f / (1.0 + volume));
		    
    // AttractonPoint
    shoals[i].flock.changeAttractionPoint
    (
     0,
     ofRandom(winWidth * 0.48, winWidth * 0.52),
     ofRandom(winHeight * 0.15, winHeight * 0.85),
     ofRandom(shoals[i].flock.maxZ + 50, shoals[i].flock.maxZ + 150),
     force * 1.3,
     winHeight * attractionPointScale
     );
    shoals[i].flock.changeAttractionPoint
    (
     1,
     ofRandom(winWidth*0.48, winWidth*0.52),
     ofRandom(winHeight*0.15, winHeight*0.85),
     ofRandom(400, 650),
     force * 1.3,
     winHeight * attractionPointScale
    );
    
    // Back to front
    for (int j=2; j <= 3; j++){
      shoals[i].flock.changeAttractionPoint
      (
       j,
       ofRandom(0, winWidth),
       ofRandom(0, winHeight),
       ofRandom(shoals[i].flock.minZ, shoals[i].flock.minZ - 120),
       0 * 20.0f * (force / MAX_FORCE),
       winHeight * 0.5
       );
    }
    
    if(paused){
      return;
    }
    
    shoals[i].update(force);
  }
  
  util.updateScreenCaptureParams();
}

//--------------------------------------------------------------
void testApp_D::draw(){
  
  util.setTitle(devMode);
  
  // 180 rotate
  // ofTranslate(winWidth/2, winHeight/2, 0);
  // glRotatef(180.f, 0.f, 1.f, 0.f);
  // glRotatef(180.f, 0.f, 0.f, 1.f);
  // ofTranslate(-winWidth/2, -winHeight/2, 0);

  if(showTrack){
    ofSetColor(0, 0, 0, 98);
    ofRect(0, 0, winWidth, winHeight);
  }

  // Draw Background
  if(Background::isMonochrome){
    ofBackground(bgColorR, bgColorG, bgColorB, 255);
  }else{
    Background::draw(winWidth, winHeight);
  }

  ofEnableBlendMode(OF_BLENDMODE_ADD);

  ofSetColor(255, 255, 255);

  #ifdef DEBUG
    // make AttractionPoint visible
    if(flock.hasAttractionPoints()){
      ofSetColor(0, 255, 255, 255 * volume);
      for(int i=0; i < flock.attractionPoints.size(); i++){
        flock.attractionPoints[i]->draw();
      }
    }
  #endif
  
  for(int i=0; i < shoals.size(); i++){
    for (int j=0; j < FISH_NUM[i]; j++) {
      Boid3d * b = shoals[i].flock.get(j);
      
      glPushMatrix();
      glTranslatef(b->x, b->y, b->z);

      // turningZ が true のとき、常に正面を向いた状態で移動する
      if(shoals[i].turningZ){
        glRotatef(atan2( b->vz, b->vx ) * -RAD_TO_DEG, 0, 1, 0);
      }
      glRotatef(atan2( b->vy, sqrt( b->vx * b->vx + b->vz * b->vz ) )*RAD_TO_DEG, 0, 0, 1);

      // draw fish
      shoals[i].drawOnBoid(b);
      
      glPopMatrix();


      if(devMode){
        ofSetColor(255, 0, 0, 255);
        ofCircle(b->x, b->y, b->z, 1);
        
        /*
        string boidParams;
        boidParams = ofToString(b->vx, 2) + " ";
        boidParams += ofToString(b->vy, 2) + " ";
        boidParams += ofToString(b->vz, 2);
        ofDrawBitmapString(boidParams, b->x, b->y, b->z);
        */
      }
    }
  }
  
  ofEnableBlendMode(OF_BLENDMODE_ALPHA);
  
  if(devMode){
    ofSetColor(255, 255, 255, 200);
    ofRect(0, 0, 400, winHeight);
    ofSetColor(0);

    ofDrawBitmapString("VOLUME", 20, 20);
    ofDrawBitmapString(ofToString(volume), 20, 40);
    
    ofDrawBitmapString("FORCE", 20, 80);
    ofDrawBitmapString(ofToString(force), 20, 100);

    ofDrawBitmapString("MAX SPEED", 20, 140);
    ofDrawBitmapString(ofToString(shoals[0].flock.getMaxSpeed()), 20, 160);
  }
}

//--------------------------------------------------------------
void testApp_D::keyPressed(int key){
  
  switch(key){
    // 'd'キー でデベロッパーモード
    // 種々のパラメータを出力
    case 'd':
      devMode = !devMode;
      break;

    // 'f'キー でフルスクリーンに切り替え
    case 'f':
      isFullscreen = !isFullscreen;
      ofSetFullscreen(isFullscreen);
      break;
      
    // 's'キー でスクリーンショット（静止画）の撮影
    case 's':
      util.saveScreenShot();
      break;
    
    // 'r'キーで録画・書き出し
    case 'r':
      util.toggleScreenRecording(winWidth, winHeight, FPS);
      break;
      
    // 入力音量を補正する
    case 44: // ','キー
      volumeBase -= 0.050f;
      break;
    case 46: // '.'キー
      volumeBase += 0.050f;
      break;
      
    //TODO: 水槽のサイズを変更する
    case 91: // '['キー
      break;
    case 93: // ']'キー
      break;
      
    // 'm'キーで画像モードの切り替え
    case 'm':
      imgMode = !imgMode;
      break;
      
    // 'h'キーで魚を非表示
    case 'h':
      Shoal::isHidden = !Shoal::isHidden;
      break;
      
    // 'b'キーで背景の切り替え
    case 'b':
      Background::isMonochrome = !Background::isMonochrome;
      break;
      
    // スペースキーで一時停止
    case ' ':
      paused = !paused;
      break;
  }
}

//--------------------------------------------------------------
void testApp_D::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp_D::mouseMoved(int x, int y ){
  // マウスカーソルが動いたとき表示させる
  cursorHidingFrameCount = 0;
  ofShowCursor();
}

//--------------------------------------------------------------
void testApp_D::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp_D::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp_D::exit() {
}

//------------------- Partial Functions ------------------------
void testApp_D::setAllBounds(){
  for(int i=0; i < shoals.size(); i++){
    shoals[i].flock.setBounds
    (
     shoals[i].flock.minX,
     shoals[i].flock.minY,
     shoals[i].minBoundZ,
     winWidth,
     winHeight,
     shoals[i].maxBoundZ
    );
  }
}


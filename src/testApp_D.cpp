#include "testApp_D.h"

const int testApp_D::FISH_NUM[] = {600, 80, 5};
const float testApp_D::MAX_FORCE = 20000.0f;
const float testApp_D::MAX_ALIGN = 12.0f;
const float testApp_D::MIN_SPEED = 4.0 * 60.0 / FPS;

//--------------------------------------------------------------
void testApp_D::setup(){
  
  ofSetFrameRate(FPS);
	ofBackground(255);
  ofSetCircleResolution(60);
  ofSetVerticalSync(true);
  ofEnableAlphaBlending();
  
  paused = false;
  devMode = false;
  imgMode = true;
  cursorHidingFrameCount = 0;
  ofHideCursor();

  // ウィンドウサイズが必要になるたびに ofGetWidth/Height を使わない
  // ここで一回だけ取得しておく
  winWidth = ofGetWidth();
  winHeight = ofGetHeight();
  
  force = 10000.0f;
  stream = 0;
  
  for(int i=0; i < (sizeof(FISH_NUM) / sizeof(FISH_NUM[0])); i++){
    vector<Fish> _vFish;
    fish.push_back(_vFish);
  }
  
  fishImages = new ofImage[fish.size()];
  
  fishImages[0].loadImage("groupD/fish0.png");
  fishImages[1].loadImage("groupD/fish1.png");
  fishImages[2].loadImage("groupD/fish2.png");
  
  for(int i=0; i < fish.size(); i++){
    for(int j=0; j < FISH_NUM[i]; j++){
      Fish _fish(& fishImages[i]);
      fish[i].push_back(_fish);
    }
  }

  cout << sizeof(FISH_NUM) / sizeof(FISH_NUM[0]) << endl;
  cout << fish.size() << endl;
  
  for(int i=0; i < fish.size(); i++){
    flocks[i].setup(FISH_NUM[i], winWidth*0.5, winHeight*0.5, 0, 0);
    flocks[i].setBoundmode(0);
    flocks[i].setBounds(0, 0, 0, winWidth, winHeight, 0);
    
    flocks[i].setMaxSpeed(MIN_SPEED);
    flocks[i].setMaxForce(MAX_FORCE * 10.0f);
    flocks[i].setMaxTurn(0.0f);
    
    flocks[i].setAlign(MAX_ALIGN);
    flocks[i].setDistAlign(90.0f);
    
    flocks[i].setCohesion(20.0f);
    flocks[i].setDistCohesion(5.0f);
    
    flocks[i].setSeparate(100.0f);
    flocks[i].setDistSeparation(80.0f);
    
    flocks[i].setAttraction(-200.0);
    flocks[i].setAttractionDev(-1.0);
    
    // AttractionPoint の id[0-3] は毎フレーム、
    // id[4-7]はウィンドウサイズが変わるたびに変更されるので、
    // ここでは引数は気にせず、追加だけしておく
    for (int j=0; j < 8; j++){
      flocks[i].addAttractionPoint(0, 0, 0, 0, 0);
    }
  }

  // ウィンドウサイズに依存する設定の初期化
  windowResized(ofGetWidth(), ofGetHeight());
  
  fbo.allocate(winWidth, winHeight);
  fbo.begin();
  {
    ofClear(255);
    Background::draw();
  }
  fbo.end();
  
  volume = 0;

  for(int i=0; i < FPS * 10; i++){
    formarVolumes.push_back(0);
  }

  for(int i=0; i < FPS * 0.2; i++){
    currentVolumes.push_back(0);
  }
  
  volumeBase = 0;
  
  soundInput.setup();
  
  for(int i=0; i < 250; i++){
    testApp_D::update();
  }
}

//--------------------------------------------------------------
void testApp_D::windowResized(int w, int h){
  winWidth = w;
  winHeight = h;
  
  for(int i=0; i < fish.size(); i++){
    flocks[i].setBounds(0, 0, 0, w, h, 600);
    
    /*
    // 端に追いやられた魚を中央へ戻すための AttractionPoint
    flocks[i].changeAttractionPoint(4, -200, 0, 300, 0.750f, h*0.5);
    flocks[i].changeAttractionPoint(5, -200, h, 300, 0.750f, h*0.5);
    flocks[i].changeAttractionPoint(6, w+200, 0, 300, 0.750f, h*0.5);
    flocks[i].changeAttractionPoint(7, w+200, h, 300, 0.750f, h*0.5);
     */
  }
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
  
  // 音量の取得
  // soundInput.getVolumeLog() は 0.0 〜 1.0 の値を返す
  volume = MAX(soundInput.getVolumeLog() + volumeBase, 0);

  float formerVolumeAverage = util.updateListAverage(& formarVolumes, volume);
  float currentVolumeAverage = util.updateListAverage(& currentVolumes, volume);
  
  for(int i=0; i < fish.size(); i++){
    // 散開するときだけ、魚の速度を上げる
    if(currentVolumeAverage > formerVolumeAverage || currentVolumeAverage > 0.10f){
      force = MAX_FORCE * currentVolumeAverage;
      flocks[i].setMaxSpeed(MIN_SPEED + 30.0f * (force / MAX_FORCE));
    }else{
      if(force < 0.000010f){
        force = 0;
      }else{
        force = force * 0.40f;
      }
      flocks[i].setMaxSpeed(MAX(flocks[i].getMaxSpeed() * 0.950, MIN_SPEED));
    }
    
    // 散開するときだけ、魚の整列を乱す
    flocks[i].setAlign(MAX_ALIGN / (1.0 + volume));
    flocks[i].setDistAlign(90.0f / (1.0 + volume));
    
    // 音量に応じて中心から斥力を発生させる、魚を散開させるための AttractonPoint
    flocks[i].changeAttractionPoint
    (
     0,
     ofRandom(winWidth * 0.48, winWidth * 0.52),
     ofRandom(winHeight * 0.15, winHeight * 0.85),
     ofRandom(flocks[i].maxZ + 50, flocks[i].maxZ + 150),
     force,
     600.0f
     );
    flocks[i].changeAttractionPoint
    (
     1,
     ofRandom(winWidth*0.48, winWidth*0.52),
     ofRandom(winHeight*0.15, winHeight*0.85),
     ofRandom(400, 650),
     force,
     500.0f
    );
    
    // 魚を手前に戻す力
    for (int j=2; j <= 3; j++){
      flocks[i].changeAttractionPoint
      (
       j,
       ofRandom(0, winWidth),
       ofRandom(0, winHeight),
       ofRandom(flocks[i].minZ, flocks[i].minZ - 120),
       0 * 20.0f * (force / MAX_FORCE),
       winHeight * 0.5
       );
    }
    
    // ポーズ中は、魚の位置を更新しない
    if(paused){
      return;
    }
    
    flocks[i].update();
    
    // 魚らしく見えるように補正
    for (int j=0; j < FISH_NUM[i]; j++) {
      // Z軸の速度に斥力をすぐに反映させ、魚が突発的に逃げているように見せる
      
      //flocks[i].get(j)->vz +=
      //ofMap(flocks[i].get(j)->z, flocks[i].minZ + 0.001, flocks[i].maxZ, 0.450f, 1.850f, true) * (force / MAX_FORCE);
      
      if(flocks[i].get(j)->vz > 0){
        flocks[i].get(j)->vz += 0.850f * (force / MAX_FORCE);
        //flocks[i].get(j)->z += 0.850f * (force / MAX_FORCE);
      }else{
        flocks[i].get(j)->vz += 0.450f * (force / MAX_FORCE);
        //flocks[i].get(j)->z -= 0.850f * (force / MAX_FORCE);
      }
      
      // 魚の身体が縦向きになると違和感を与えるので、
      // できるだけ横向きになるようにY方向を調整
      flocks[i].get(j)->vy = MIN(flocks[i].get(j)->vy, 0.620f);
      flocks[i].get(j)->vy = MAX(flocks[i].get(j)->vy, -0.620f);
    }
  }
  
  util.updateScreenCaptureParams();
}

//--------------------------------------------------------------
void testApp_D::draw(){
  ofBackground(0, 0, 0, 255);
  //ofBackground(255, 255, 255, 55);

  util.setTitle(devMode);
  
  ofSetColor(255, 255, 255);
  // 背景の描画
  //fbo.draw(0, 0, winWidth, winHeight);

  #ifdef DEBUG
    // AttractionPoint の描画
    if(flock.hasAttractionPoints()){
      ofSetColor(0, 255, 255, 255 * volume);
      for(int i=0; i < flock.attractionPoints.size(); i++){
        flock.attractionPoints[i]->draw();
      }
    }
  #endif
  
  //ofEnableBlendMode(OF_BLENDMODE_A);
  
  for(int i=0; i < fish.size(); i++){
    for (int j=0; j < FISH_NUM[i]; j++) {
      Boid3d * b = flocks[i].get(j);
      
      glPushMatrix();
      {
        glTranslatef(b->x, b->y, b->z);
        
        //glRotatef(atan(b->vz) * -RAD_TO_DEG, 0, 1, 0);
        //glRotatef(atan2(b->vy, b->vx) * RAD_TO_DEG, 0, 0, 1);
        
        glRotatef( atan2( b->vz, b->vx )* -RAD_TO_DEG, 0, 1, 0 );
        glRotatef( atan2( b->vy, sqrt( b->vx * b->vx + b->vz * b->vz ) )*RAD_TO_DEG, 0, 0, 1 );
        
        // 魚のグラフィックの描画
        if(imgMode){
          fish[i][j].drawImage(b);
        }else{
          fish[i][j].draw(b);
        }
      }
      glPopMatrix();
      
      if(devMode){
        ofSetColor(0);
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
    ofRect(0, 0, 200, winHeight);
    ofSetColor(0);

    ofDrawBitmapString("VOLUME", 20, 20);
    ofDrawBitmapString(ofToString(volume), 20, 40);
    
    ofDrawBitmapString("FORCE", 20, 80);
    ofDrawBitmapString(ofToString(force), 20, 100);

    ofDrawBitmapString("MAX SPEED", 20, 140);
    ofDrawBitmapString(ofToString(flocks[0].getMaxSpeed()), 20, 160);
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
      ofToggleFullscreen();
      break;
      
    // 's'キー でスクリーンショット（静止画）の撮影
    case 's':
      util.saveScreenShot();
      break;
      
    // 入力音量を補正する
    case 44: // ','キー
      volumeBase -= 0.050f;
      break;
    case 46: // '.'キー
      volumeBase += 0.050f;
      break;
    
    // 'r'キーで録画・書き出し
    case 'r':
      util.toggleScreenRecording(winWidth, winHeight, FPS);
      
      break;
    
    // 'm'キーで画像モードの切り替え
    case 'm':
      imgMode = !imgMode;
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
  util.stopRecordScreen();
}

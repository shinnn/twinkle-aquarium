#include "Shoal.h"

float Shoal::defaultScale = 1.0f;
float Shoal::defaultAlign = 12.0f;
float Shoal::defaultSpeed = 4.0f;

float Shoal::defaultMinBoundZ = 0;
float Shoal::defaultMaxBoundZ = 600;

float Shoal::defaultEscapeSpeed = 50.0f;
float Shoal::maxForce = 20000.0f;

float Shoal::additional_vx = 0.04250f;
float Shoal::additional_vy = 0.04250f;
float Shoal::additional_vz = 0.850f;

bool Shoal::isHidden = false;

Shoal::Shoal(ofImage ofI) {
  img = ofI;
  imgWidth = img.getWidth();
  imgHeight = img.getHeight();

  scale = 1.0f;
  speed = defaultSpeed;
  align = defaultAlign;
  
  minBoundZ = defaultMinBoundZ;
  maxBoundZ = defaultMaxBoundZ;
  
  turningZ = true;

  alpha = 255;
  saturation = -1;
  brightness = -1;
}

void Shoal::setupBoids(int num, int _w, int _h) {

  flock.setup(num, _w * 0.50f, _h * 0.50f, 0, 1);
  flock.setBoundmode(0);
  flock.setBounds(0, 0, 0, _w, _h, 0);
  
  flock.setMaxSpeed(speed);
  flock.setMaxForce(maxForce * 10.0f);
  flock.setMaxTurn(0.0f);
  
  flock.setAlign(align);
  flock.setDistAlign(90.0f);
  
  flock.setCohesion(20.0f);
  flock.setDistCohesion(5.0f);
  
  flock.setSeparate(100.0f);
  flock.setDistSeparation(80.0f);
  
  flock.setAttraction(-200.0);
  flock.setAttractionDev(-1.0);
  
  // create attraction points
  for (int i=0; i < 8; i++) {
    flock.addAttractionPoint(0, 0, 0, 0, 0);
  }
}

void Shoal::update(double force) {
  flock.update();
  
  // Z軸の速度に斥力をすぐに反映させ、魚が突発的に逃げているように見せる
  for (int i=0; i < flock.size(); i++) {
    
    if (flock.get(i)->vx > 0) {
      flock.get(i)->vx += additional_vx * (force / maxForce);
    } else {
      flock.get(i)->vx -= additional_vx * (force / maxForce);
    }
    
    if (flock.get(i)->vy > 0) {
      flock.get(i)->vy += additional_vy * (force / maxForce);
    } else {
      flock.get(i)->vy -= additional_vy * (force / maxForce);
    }
    
    if (flock.get(i)->vz > 0) {
      flock.get(i)->vz += additional_vz * (force / maxForce);
    } else {
      flock.get(i)->vz += additional_vz * 0.50f * (force / maxForce);
    }
    
    /*
    flock.get(i)->vz += ofMap
    (
     flock.get(i)->z,
     flock.minZ + 0.001,
     flock.maxZ,
     0.450f,
     1.850f,
     true
     ) * (force / maxForce);
    */
    
    // 魚の身体が縦向きになると違和感を与えるので、
    // できるだけ横向きになるようにY方向を調整
    flock.get(i)->vy = MIN(flock.get(i)->vy, 0.620f);
    flock.get(i)->vy = MAX(flock.get(i)->vy, -0.620f);
    
    /*
     // 魚が平べったく見えすぎないようにする
     if(-0.75 < flocks[i].get(j)->vx && flocks[i].get(j)->vx < 0.75){
     if(flocks[i].get(j)->vx > 0){
     flocks[i].get(j)->vx = 1;
     }else{
     flocks[i].get(j)->vx = -1;
     }
     }
    */
  }
}

void Shoal::drawOnBoid(Boid3d * b) {
  
  if (!isHidden) {
    ofColor _color
    (
     170 + b->vx * 75,
     170 + b->vy * 75,
     200 + b->vz * 55,
     ofMap(b->z, 0, 400, 128, 255, true) + alpha - 255
     );
    
    //_color.setSaturation((abs(b->vx)+abs(b->vy)+abs(b->vz))*200*0.333);
    _color.setBrightness( ofMap((abs(b->vx)+abs(b->vy)+abs(b->vz)), 0, 2.8, 0, 255, true) );
    
    if (saturation >= 0) {
      _color.setSaturation(saturation);
    }
    if (brightness >= 0) {
      _color.setBrightness(brightness);
    }
    
    ofSetColor(_color);
    
    float _scale = 0.250f * defaultScale * scale;
    ofScale(_scale, _scale);
    
    img.draw
    (
     - imgWidth * 0.75,
     - imgHeight * 0.5,
     imgWidth,
     imgHeight
     );
  }

  return b;
}

Bubble::Bubble(ofImage ofI): Shoal(ofI) {
  speed = 3.20f;
  align = 4.0f;
  
  maxBoundZ = 800;
  
  alpha = 200;
  saturation = 120;
  brightness = 255;
  
  turningZ = false;
}

#include "Fish.h"

Fish::Fish(){
}

Fish::Fish(ofImage * ofI){
  setImage(ofI);
}

void Fish::setImage(ofImage * ofI){
  img = ofI;
  imgWidth = img->getWidth();
  imgHeight = img->getHeight();
}

void Fish::draw(Boid3d * b){
  ofColor _color
  (
    175 - b->vx * 75,
    200 - b->vy * 55,
    200 - b->vz * 55,
    230
  );
  
  //_color.setSaturation((abs(b->vx)+abs(b->vy)+abs(b->vz))*200*0.333);
  _color.setBrightness(100 + (abs(b->vx)+abs(b->vy)+abs(b->vz))*100*0.333);
  
  ofSetColor(_color);

  ofBeginShape();
  {
    ofVertex(0, 6);
    ofVertex(8, 0);
    ofVertex(0, -6);
    ofVertex(-38, 0);
  }
  ofEndShape();
  
  ofPushMatrix();
  {
    ofTranslate(-42, 0);
    ofBeginShape();
    {
      ofVertex(0, 4);
      ofVertex(16, 0);
      ofVertex(0, -4);
      ofVertex(4, 0);
    }
    ofEndShape();
  }
  ofPopMatrix();
};

void Fish::drawImage(Boid3d * b){
  ofColor _color
  (
   170 + b->vx * 75,
   170 + b->vy * 75,
   200 + b->vz * 55,
   ofMap(b->z, 0, 400, 80, 255, true)
  );
  
  //_color.setSaturation((abs(b->vx)+abs(b->vy)+abs(b->vz))*200*0.333);
  _color.setBrightness( ofMap((abs(b->vx)+abs(b->vy)+abs(b->vz)), 0, 2.8, 0, 255, true) );

  ofSetColor(_color);
  
  ofScale(0.25, 0.25);

  if(img) img->draw
  (
   - imgWidth * 0.75,
   - imgHeight * 0.5,
   imgWidth,
   imgHeight
  );
}

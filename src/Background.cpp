#include "Background.h"

bool Background::isMonochrome = false;

int Background::x = 0;
int Background::y = 20;

void Background::draw(int w, int h) {
  if (ofRandomf() > 0.5) {
    x += 12;
  } else {
    x -= 12;
  }
  
  if (x <= 0) {
    x = 0;
  }
  
  glBegin(GL_POLYGON);
  {
    ofSetColor(ofColor::fromHsb(50, 120, 140));
    // right-top (0, 0)
    glVertex3f(0.0f, 0.0f, 0.0f);

    ofSetColor(ofColor::fromHsb(150, 110, 255));
    // left-top (w, 0)
    glVertex3f(w, 0.0f, 0.0f);
    
    ofSetColor(ofColor::fromHsb(130, 220, 120));
    // left-bottom (w, h)
    glVertex3f(w, h, 0.0f);
    
    ofSetColor(ofColor::fromHsb(130, 220, 120));

    // right-bottom (0, h)
    glVertex3f(0.0f, h, 0.0f);
  }
  glEnd();
}

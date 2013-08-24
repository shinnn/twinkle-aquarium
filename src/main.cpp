#include "ofMain.h"
#include "testApp_D.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main(){
  ofAppGlutWindow window;
	
  ofSetupOpenGL(&window, 1414, 896, OF_WINDOW); // 展示のフォーマットが 1414 × 896 である
  
  //------- HD mode
  //ofSetupOpenGL(&window, 1920, 1080, OF_WINDOW);
  //-------

  //------- fullscreen mode
	//ofSetupOpenGL(&window, 1414, 896, OF_FULLSCREEN);
  //-------
	
  // this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp_D());
}

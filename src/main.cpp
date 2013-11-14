#include "ofMain.h"
#include "App.h"

//==============================================================
int main(){
	
  //------- HD mode
  ofSetupOpenGL(1920, 1080, OF_WINDOW);
  //-------
  
  //------- fullscreen mode
	//ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);
  //-------
	
  // 5 display mode
  //ofSetupOpenGL(&window, 8386, 896, OF_WINDOW);
  //-------
  
  // this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new App());
}

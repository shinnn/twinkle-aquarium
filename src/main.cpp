#include "ofMain.h"
#include "app.h"
#include "ofAppGlutWindow.h"

//==============================================================
int main(){
  ofAppGlutWindow window;
	
  //------- HD mode
  ofSetupOpenGL(&window, 1920, 1080, OF_WINDOW);
  //-------
  
  //------- fullscreen mode
	//ofSetupOpenGL(&window, 1414, 896, OF_FULLSCREEN);
  //-------
	
  //ofSetupOpenGL(&window, 1414, 896, OF_WINDOW);
  
  // 5 display mode
  //ofSetupOpenGL(&window, 8386, 896, OF_WINDOW);
  //-------
  
  // this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new app());
}

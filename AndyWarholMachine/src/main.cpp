#include "ofMain.h"
#include "testApp.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800, 600, OF_FULLSCREEN);
	ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
	ofRunApp(new testApp());
}

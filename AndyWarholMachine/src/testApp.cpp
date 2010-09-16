#include "testApp.h"

void testApp::setup(){
	int camWidth = 640;
	int camHeight = 480;	
	cameraFrames = 0;
	camera.initGrabber(camWidth, camHeight);
	background.setup(camWidth, camHeight);
	difference.setup(camWidth, camHeight);
	
	avgGrapher.setup(160, 60, 0, 64);
	devGrapher.setup(160, 60, 0, 64);
}

void testApp::update(){
	camera.update();
	if(camera.isFrameNew()) {
		if(cameraFrames < 10)
			background.set(camera);
		else
			background.lerp(.01, camera);
		background.update();
		
		difference.makeAbsoluteDifference(background, camera);
		difference.update();
		avgGrapher.addValue(difference.getAverage());
		devGrapher.addValue(difference.getStandardDeviation());
		
		cameraFrames++;
	}
}

void testApp::draw(){
	ofSetColor(255, 255, 255);
	camera.draw(0, 0, 320, 240);
	background.draw(320, 0, 320, 240);
	difference.draw(640, 0, 320, 240);
	
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	
	ofTranslate(640, 240);
	avgGrapher.draw(0, 0);
	devGrapher.draw(160, 0);
}

void testApp::keyPressed(int key){
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mouseDragged(int x, int y, int button){
}

void testApp::mousePressed(int x, int y, int button){
}

void testApp::mouseReleased(int x, int y, int button){
}

void testApp::windowResized(int w, int h){
}


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
	
	// panel wtf
	panel.setup("Control Panel", 10, 10, 300, 600);
	panel.addPanel("Main", 1);
	panel.setWhichPanel("Main");
	panel.addToggle("Set Low Threshold", "setLowThreshold", false);
	panel.addDrawableRect("Difference Average", &avgGrapher, 160, 60);
	panel.addDrawableRect("Difference Deviation", &devGrapher, 160, 60);
}

void testApp::update(){
	camera.update();
	if(camera.isFrameNew()) {
		if(cameraFrames < 10)
			background.set(camera);
		else
			background.lerp(1. / 1000., camera);
		background.update();
		
		difference.makeAbsoluteDifference(background, camera);
		difference.update();
		avgGrapher.addValue(difference.getAverage());
		devGrapher.addValue(difference.getStandardDeviation());
		
		cameraFrames++;
	}
}

void testApp::draw(){
	ofPushStyle();
	ofPushMatrix();
	ofSetColor(255, 255, 255);
	camera.draw(0, 0, 320, 240);
	background.draw(320, 0, 320, 240);
	difference.draw(640, 0, 320, 240);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	ofPopMatrix();
	ofPopStyle();
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


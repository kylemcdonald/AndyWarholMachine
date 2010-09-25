#include "testApp.h"

void testApp::setup(){
	int camWidth = 640;
	int camHeight = 480;	
	cameraFrames = 0;
	camera.initGrabber(camWidth, camHeight);
	background.setup(camWidth, camHeight);
	difference.setup(camWidth, camHeight);
	
	avgGrapher.setup(180, 60, 0, 60);
	devGrapher.setup(180, 60, 0, 60);
	
	// panel wtf
	panel.setup("Control Panel", 10, 10, 300, 700);
	panel.addPanel("Main", 1);
	panel.setWhichPanel("Main");
	panel.addSlider("Adapt Speed", "adaptSpeed", 1000, 1, 1000, false);
	panel.addToggle("Set Low Threshold", "setLowThreshold", false);
	panel.addDrawableRect("Difference Average", &avgGrapher, 180, 60);
	panel.addSlider("Average Low Threshold", "avgMinThreshold", 0, 0, 60, true);
	panel.addSlider("Average High Threshold", "avgMaxThreshold", 0, 0, 60, true);
	panel.addToggle("Average Status", "avgStatus", false);
	panel.addDrawableRect("Difference Deviation", &devGrapher, 180, 60);
	panel.addSlider("Deviation Low Threshold", "devMinThreshold", 0, 0, 60, true);
	panel.addSlider("Deviation High Threshold", "devMaxThreshold", 0, 0, 60, true);
	panel.addToggle("Deviation Status", "devStatus", false);
	panel.addToggle("Combined Status", "combinedStatus", false);
}

void testApp::update(){
	camera.update();
	if(camera.isFrameNew()) {
		if(cameraFrames < 10)
			background.set(camera);
		else
			background.lerp(1. / panel.getValueF("adaptSpeed"), camera);
		background.update();
		
		difference.makeAbsoluteDifference(background, camera);
		difference.update();
		avgGrapher.addValue(difference.getAverage());
		devGrapher.addValue(difference.getStandardDeviation());
		
		cameraFrames++;
	}
	
	avgGrapher.setMinThreshold(panel.getValueI("avgMinThreshold"));
	avgGrapher.setMaxThreshold(panel.getValueI("avgMaxThreshold"));
	devGrapher.setMinThreshold(panel.getValueI("devMinThreshold"));
	devGrapher.setMaxThreshold(panel.getValueI("devMaxThreshold"));
	
	panel.setValueB("avgStatus", avgGrapher.getStatus());
	panel.setValueB("devStatus", devGrapher.getStatus());
	panel.setValueB("combinedStatus", avgGrapher.getStatus() && devGrapher.getStatus());
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


#include "testApp.h"

void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	cameraFpsTimer.setSmoothing(.9);
	appFpsTimer.setSmoothing(.9);
	cameraFpsGrapher.setup(180, 60, 0, 60);
	appFpsGrapher.setup(180, 60, 0, 800);
	
	int camWidth = 640;
	int camHeight = 480;	
	int camFramerate = 30;
	maxDelay = 10;
	
	cameraFrames = 0;
	camera.initGrabber(camWidth, camHeight);
	camera.setDesiredFrameRate(30);
	cameraReady = false;
	background.setup(camWidth, camHeight);
	difference.setup(camWidth, camHeight);
	
	avgGrapher.setup(180, 60, 0, 60);
	devGrapher.setup(180, 60, 0, 60);
	
	videoDelay.setup(camWidth, camHeight, camFramerate * maxDelay);
	curDelay.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
	
	// panel setup
	panel.setup("Control Panel", 10, 10, 300, 700);
	panel.addPanel("Main", 1);
	panel.setWhichPanel("Main");
	panel.addSlider("Adapt Speed", "adaptSpeed", 10, 1, 12, false);
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
	
	panel.addPanel("Delay", 1);
	panel.setWhichPanel("Delay");
	panel.addSlider("Delay Amount", "delayAmount", 0, 0, (maxDelay * camFramerate) - 1, true);
	panel.addSlider("Playback Framerate", "playbackFramerate", 10, 1, 60, true);
	panel.addDrawableRect("Camera Framerate", &cameraFpsGrapher, 180, 60);
	panel.addDrawableRect("App Framerate", &appFpsGrapher, 180, 60);

}

void testApp::update(){
	camera.update();
	if(camera.isFrameNew()) {
		if(cameraFrames < 10) {
			background.set(camera);
			cameraReady = false;
		} else {
			background.lerp(2. / powf(2., panel.getValueF("adaptSpeed")), camera);
			cameraReady = true;
		}
		background.update();
		
		videoDelay.add(camera);
		
		difference.makeAbsoluteDifference(background, camera);
		difference.update();
		avgGrapher.addValue(difference.getAverage());
		devGrapher.addValue(difference.getStandardDeviation());
		
		cameraFrames++;
		
		cameraFpsTimer.tick();
		
		cameraFpsGrapher.addValue(cameraFpsTimer.getFramerate());
		appFpsGrapher.addValue(appFpsTimer.getFramerate());
	}
	
	delayTimer.setFramerate(panel.getValueI("playbackFramerate"));
	if(delayTimer.tick() && cameraReady) {
		int delayAmount = panel.getValueI("delayAmount");
		int n = curDelay.getWidth() * curDelay.getHeight() * 3;
		memcpy(curDelay.getPixels(), videoDelay.read(delayAmount).getPixels(), n);
		curDelay.update();
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
	camera.draw(320 * 0, 0, 320, 240);
	background.draw(320 * 1, 0, 320, 240);
	difference.draw(320 * 2, 0, 320, 240);
	curDelay.draw(320 * 3, 0, 320, 240);
	ofPopMatrix();
	ofPopStyle();
	
	appFpsTimer.tick();
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


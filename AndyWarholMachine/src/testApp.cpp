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
	
	videoSaver.setup(camWidth, camHeight, "output.mov");
	
	avgGrapher.setup(180, 60, 0, 60);
	devGrapher.setup(180, 60, 0, 60);
	
	videoDelay.setup(camWidth, camHeight, camFramerate * maxDelay);
	curDelay.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
	
	// panel setup
	panel.setup("Control Panel", 10, 10, 500, 720);
	panel.addPanel("Detection", 2);
	panel.setWhichPanel("Detection");
	panel.setWhichColumn(0);
	panel.addSlider("Adapt Speed", "adaptSpeed", 10, 1, 12, false);
	panel.addDrawableRect("Difference Average", &avgGrapher, 180, 60);
	panel.addSlider("Average Threshold", "avgThreshold", 0, 0, 60, true);
	panel.addToggle("Average Status", "avgStatus", false);
	panel.addDrawableRect("Difference Deviation", &devGrapher, 180, 60);
	panel.addSlider("Deviation Threshold", "devThreshold", 0, 0, 60, true);
	panel.addToggle("Deviation Status", "devStatus", false);
	panel.setWhichColumn(1);
	panel.addDrawableRect("Camera Input", &camera, 240, 180);
	panel.addDrawableRect("Background", &background, 240, 180);
	panel.addDrawableRect("Difference", &difference, 240, 180);
	
	panel.addPanel("Recording", 1);
	panel.setWhichPanel("Recording");
	panel.addToggle("Use Manual Presence", "useManualPresence", false);
	panel.addToggle("Manual Presence", "manualPresence", false);
	panel.addToggle("Automatic Presence", "automaticPresence", false);
	panel.addToggle("Presence", "presence", false);
	
	panel.addPanel("Delay", 1);
	panel.setWhichPanel("Delay");
	panel.setWhichColumn(0);
	panel.addSlider("Delay Amount", "delayAmount", 0, 0, (maxDelay * camFramerate) - 1, true);
	panel.addSlider("Playback Framerate", "playbackFramerate", 20, 1, 60, true);
	panel.addDrawableRect("Camera Framerate", &cameraFpsGrapher, 180, 60);
	panel.addDrawableRect("App Framerate", &appFpsGrapher, 180, 60);
	panel.addSlider("Write Position", "writePosition", 0, 0, 1, false);
	panel.addSlider("Read Position", "readPosition", 0, 0, 1, false);
	
	panel.addPanel("Output", 1);
	panel.setWhichPanel("Output");
	panel.addToggle("Letterbox Video", "letterboxVideo", false);
}

void testApp::update(){
	camera.update();
	if(camera.isFrameNew()) {
		if(cameraFrames++ < 10) {
			cameraReady = false;
			background.set(camera);
		} else {
			cameraReady = true;
			background.lerp(2. / powf(2., panel.getValueF("adaptSpeed")), camera);
			
			if(cameraFpsTimer.getFramerate() < (15 + 30) / 2)
				videoDelay.add(camera); // double up frames for 15 fps
			videoDelay.add(camera);
			
			cameraFpsTimer.tick();
		}
		
		if(cameraFrames < 30 * 10) {
			videoSaver.addFrame(camera.getPixels(), 1. / 30);
		} else if(cameraFrames == 30 * 10) {
			videoSaver.finishMovie();
		}
		
		background.update();
		
		difference.makeAbsoluteDifference(background, camera);
		difference.update();
		avgGrapher.addValue(difference.getAverage());
		devGrapher.addValue(difference.getStandardDeviation());
		
		if(cameraFrames > 1) {
			cameraFpsGrapher.addValue(cameraFpsTimer.getFramerate());
			appFpsGrapher.addValue(appFpsTimer.getFramerate());
		}
	}
	
	delayTimer.setFramerate(panel.getValueI("playbackFramerate"));
	if(delayTimer.tick() && cameraReady) {
		int delayAmount = panel.getValueI("delayAmount");
		int n = curDelay.getWidth() * curDelay.getHeight() * 3;
		memcpy(curDelay.getPixels(), videoDelay.read().getPixels(), n);
		curDelay.update();
	}
	
	avgGrapher.setThreshold(panel.getValueI("avgThreshold"));
	devGrapher.setThreshold(panel.getValueI("devThreshold"));
	
	panel.setValueB("avgStatus", avgGrapher.getStatus());
	panel.setValueB("devStatus", devGrapher.getStatus());
	panel.setValueB("automaticPresence", avgGrapher.getStatus() && devGrapher.getStatus());
	
	if(panel.getValueB("useManualPresence"))
		panel.setValueB("presence", panel.getValueB("manualPresence"));
	else
		panel.setValueB("presence", panel.getValueB("automaticPresence"));
	
	panel.setValueF("writePosition", videoDelay.getWritePosition());
	panel.setValueF("readPosition", videoDelay.getReadPosition());
}

void testApp::draw(){
	ofPushStyle();
	ofPushMatrix();
	
	ofBackground(0, 0, 0);
	
	float imgWidth = curDelay.getWidth();
	float imgHeight = curDelay.getHeight();
	float imgAspect = imgWidth / imgHeight;
	float scrWidth = ofGetWidth();
	float scrHeight = ofGetHeight();
	curDelay.setAnchorPercent(.5, .5);	
	glPushMatrix();
	glTranslatef(scrWidth / 2, scrHeight / 2, 0);
	if(panel.getValueB("letterboxVideo"))
		curDelay.draw(0, 0, scrHeight * imgAspect, scrHeight);
	else
		curDelay.draw(0, 0, scrWidth, scrWidth / imgAspect);
	glPopMatrix();
	
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


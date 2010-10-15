#include "testApp.h"

void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	cameraFpsTimer.setSmoothing(.9);
	appFpsTimer.setSmoothing(.9);
	cameraFpsGrapher.setup(180, 60, 0, 30);
	appFpsGrapher.setup(180, 60, 30, 120);
	
	camWidth = 640;
	camHeight = 480;	
	camFramerate = 30;
	maxDelay = 10;
	
	cameraFrames = 0;
	camera.initGrabber(camWidth, camHeight);
	cameraTimer.setFramerate(camFramerate);
	cameraReady = false;
	background.setup(camWidth, camHeight);
	difference.setup(camWidth, camHeight);
	
	presenceWait.setDelay(1);
	recording = false;
	
	avgGrapher.setup(180, 60, 0, 30);
	devGrapher.setup(180, 60, 0, 30);
	
	videoDelay.setup(camWidth, camHeight, camFramerate * maxDelay);
	curDelay.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
	
	setupControlPanel();
	
	updateArchive();
	randomArchive();
}

void testApp::setupControlPanel() {
	// panel setup
	panel.setup("Control Panel", 10, 10, 740, 850);
	panel.addPanel("Detection", 3);
	panel.setWhichPanel("Detection");
	
	panel.setWhichColumn(0);
	panel.addSlider("Adapt Speed", "adaptSpeed", 10, 1, 12, false);
	panel.addToggle("Reset Background", "resetBackground", false);
	panel.addDrawableRect("Difference Average", &avgGrapher, 180, 60);
	panel.addSlider("Average Threshold", "avgThreshold", 0, 0, 60, true);
	panel.addToggle("Average Status", "avgStatus", false);
	panel.addDrawableRect("Difference Deviation", &devGrapher, 180, 60);
	panel.addSlider("Deviation Threshold", "devThreshold", 0, 0, 60, true);
	panel.addToggle("Deviation Status", "devStatus", false);
	panel.addToggle("Use Manual Presence", "useManualPresence", true);
	panel.addToggle("Manual Presence", "manualPresence", false);
	panel.addToggle("Automatic Presence", "automaticPresence", false);
	panel.addToggle("Raw Presence", "rawPresence", false);
	panel.addToggle("Presence", "presence", false);
	panel.addToggle("Interaction Mode", "interactionMode", false);
	
	panel.setWhichColumn(1);
	panel.addDrawableRect("Camera Input", &camera, 240, 180);
	panel.addDrawableRect("Background", &background, 240, 180);
	panel.addDrawableRect("Difference", &difference, 240, 180);
	
	panel.setWhichColumn(2);	
	panel.addSlider("Start Delay", "startDelay", 4, 0, maxDelay, false);
	panel.addSlider("Stop Delay", "stopDelay", 2, 0, maxDelay, false);
	panel.addSlider("Playback Framerate", "playbackFramerate", 20, 1, 60, true);
	panel.addDrawableRect("Camera Framerate", &cameraFpsGrapher, 180, 60);
	panel.addDrawableRect("App Framerate", &appFpsGrapher, 180, 60);
	panel.addSlider("Write Position", "writePosition", 0, 0, 1, false);
	panel.addSlider("Read Position", "readPosition", 0, 0, 1, false);
}

void testApp::update(){
	// check for a new frame and update adaptive background
	camera.update();
	if(camera.isFrameNew()) {
		if(cameraFrames++ < 2 || panel.getValueB("resetBackground")) {
			cameraReady = false;
			background.set(camera);
			panel.setValueB("resetBackround", false);
		} else {
			cameraReady = true;
			background.lerp(2. / powf(2., panel.getValueF("adaptSpeed")), camera);
			cameraFpsTimer.tick();
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
	
	avgGrapher.setThreshold(panel.getValueI("avgThreshold"));
	devGrapher.setThreshold(panel.getValueI("devThreshold"));
	
	panel.setValueB("avgStatus", avgGrapher.getStatus());
	panel.setValueB("devStatus", devGrapher.getStatus());
	panel.setValueB("automaticPresence", avgGrapher.getStatus() && devGrapher.getStatus());
	
	panel.setValueB("rawPresence", panel.getValueB(panel.getValueB("useManualPresence")  ? "manualPresence" : "automaticPresence"));	
	presenceWait.set(panel.getValueB("rawPresence"));
	panel.setValueB("presence", presenceWait.get());
	
	if(presenceWait.wasTriggered()) {
		stringstream filename;
		filename << "screenTests/" << (unsigned long) time(NULL) << ".mov";
		ofLog(OF_LOG_VERBOSE, "Started recording to " + filename.str());
		videoSaver.setup(camWidth, camHeight, filename.str());
		recording = true;
		videoDelay.setReadFromWrite();
	}	
	
	if(presenceWait.wasUntriggered()) {
		videoSaver.finishMovie();
		ofLog(OF_LOG_VERBOSE, "Stopped recording");
		recording = false;
		randomArchive();
		updateArchive();
	}
	
	// set interaction mode based on presence plus some duration of time
	if(presenceWait.get() && presenceWait.length() > panel.getValueF("startDelay"))
		panel.setValueB("interactionMode", true);
	if(!presenceWait.get() && presenceWait.length() > panel.getValueF("stopDelay"))
		panel.setValueB("interactionMode", false);
	
	if(curArchive.getIsMovieDone())
		randomArchive();
	
	// add new frames to video delay and video saver
	if(cameraTimer.tick()) {
		videoDelay.add(camera);
		if(recording)
			videoSaver.addFrame(camera.getPixels(), 1. / camFramerate);
	}
	
	delayTimer.setFramerate(panel.getValueI("playbackFramerate"));
	if(delayTimer.tick() && cameraReady && panel.getValueB("interactionMode")) {
		int n = curDelay.getWidth() * curDelay.getHeight() * 3;
		memcpy(curDelay.getPixels(), videoDelay.read().getPixels(), n);
		curDelay.update();
	}
	
	panel.setValueF("writePosition", videoDelay.getWritePosition());
	panel.setValueF("readPosition", videoDelay.getReadPosition());
}

void testApp::draw(){
	ofPushStyle();
	ofPushMatrix();
	
	ofBackground(0, 0, 0);
	
	float camAspect = (float) camWidth / camHeight;
	float scrWidth = ofGetWidth();
	float scrHeight = ofGetHeight();
	glPushMatrix();
	
	glTranslatef(scrWidth / 2, scrHeight / 2, 0);
	if(panel.getValueB("interactionMode")) {
		curDelay.setAnchorPercent(.5, .5);
		curDelay.draw(0, 0, scrWidth, scrWidth / camAspect);
	} else {
		curArchive.setAnchorPercent(.5, .5);
		curArchive.draw(0, 0, scrWidth, scrWidth / camAspect);
	}
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

void testApp::exit() {
	if(recording)
		videoSaver.finishMovie();
}

void testApp::updateArchive() {
	archiveSize = archive.listDir("screenTests");
}

void testApp::randomArchive() {
	if(archiveSize > 0) {
		string filename = archive.getPath((int) ofRandom(0, archiveSize));
		curArchive.loadMovie(filename);
		curArchive.play();
		curArchive.setLoopState(false);
		curArchive.setSpeed(panel.getValueF("playbackFramerate") / (float) camFramerate);
	}
}

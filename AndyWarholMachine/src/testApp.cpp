#include "testApp.h"

void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);

	cameraFpsTimer.setSmoothing(.9);
	appFpsTimer.setSmoothing(.9);
	cameraFpsGrapher.setup(180, 60, 0, 30);
	appFpsGrapher.setup(180, 60, 30, 120);
											
	camWidth = 640;
	camHeight = 480;	
	int camFramerate = 30;
	maxDelay = 10;
	
	cameraFrames = 0;
	camera.initGrabber(camWidth, camHeight);
	cameraTimer.setFramerate(30);
	cameraReady = false;
	background.setup(camWidth, camHeight);
	difference.setup(camWidth, camHeight);
	
	presenceWait.setDelay(1);
	recording = false;
	curArchive.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
	
	avgGrapher.setup(180, 60, 0, 30);
	devGrapher.setup(180, 60, 0, 30);
	
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
	panel.addToggle("Raw Presence", "rawPresence", false);
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
		if(cameraFrames++ < 2) {
			cameraReady = false;
			background.set(camera);
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
	
	if(cameraTimer.tick()) {
		videoDelay.add(camera);
		videoSaver.addFrame(camera.getPixels(), 1. / 30);
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
	
	panel.setValueB("rawPresence", panel.getValueB(panel.getValueB("useManualPresence")  ? "manualPresence" : "automaticPresence"));	
	presenceWait.set(panel.getValueB("rawPresence"));
	panel.setValueB("presence", presenceWait.get());
	
	if(presenceWait.wasTriggered()) {
		stringstream filename;
		filename << "screenTests/" << (unsigned long) time(NULL) << ".mov";
		ofLog(OF_LOG_VERBOSE, "Started recording to " + filename.str());
		videoSaver.setup(camWidth, camHeight, filename.str());
		recording = true;
	}	
	
	if(presenceWait.wasUntriggered()) {
		videoSaver.finishMovie();
		ofLog(OF_LOG_VERBOSE, "Stopped recording");
		recording = false;
	}
	
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
	glPushMatrix();
	
	ofImage& target = panel.getValueB("presence") ? curDelay : curArchive;
		
	glTranslatef(scrWidth / 2, scrHeight / 2, 0);
	target.setAnchorPercent(.5, .5);	
	if(panel.getValueB("letterboxVideo"))
		target.draw(0, 0, scrHeight * imgAspect, scrHeight);
	else
		target.draw(0, 0, scrWidth, scrWidth / imgAspect);
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


#pragma once

#include "ofMain.h"
#include "FloatImage.h"
#include "DifferenceImage.h"
#include "Grapher.h"
#include "VideoDelay.h"
#include "ofxAutoControlPanel.h"
#include "DelayTimer.h"
#include "RateTimer.h"
#include "ofxQtVideoSaver.h"
#include "Hysterisis.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	int camWidth, camHeight;
	
	ofVideoGrabber camera;
	DelayTimer cameraTimer;
	RateTimer cameraFpsTimer, appFpsTimer;
	Grapher cameraFpsGrapher, appFpsGrapher;
	bool cameraReady;
	FloatImage background;
	DifferenceImage difference;
	
	Grapher avgGrapher, devGrapher;
	
	unsigned long cameraFrames;
	
	ofxAutoControlPanel panel;
	
	Hysterisis presenceWait;
	VideoDelay videoDelay;
	ofImage curDelay;
	int maxDelay;
	DelayTimer delayTimer;
	
	ofxQtVideoSaver videoSaver;
	bool recording;
	ofImage curArchive;
};

#pragma once

#include "ofMain.h"

class RateTimer {
protected:
	float lastTick;
	float averagePeriod;
	float smoothing;
	bool secondTick;
public:
	RateTimer() {
		reset();
	}
	void reset() {
		lastTick = 0;
		secondTick = false;
	}
	void setSmoothing(float smoothing) {
		this->smoothing = smoothing;
	}
	float getFramerate() {
		return 1 / averagePeriod;
	}
	void tick() {
		float curTick = ofGetElapsedTimef();
		if(lastTick == 0) {
			secondTick = true;
		} else {
			float curDiff = curTick - lastTick;;
			if(secondTick) {
				averagePeriod = curDiff;
				secondTick = false;
			} else {
				averagePeriod = ofLerp(curDiff, averagePeriod, smoothing);
			}
		}
		lastTick = curTick;
	}
};
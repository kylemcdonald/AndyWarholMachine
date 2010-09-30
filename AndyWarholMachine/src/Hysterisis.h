#pragma once

#include "ofMain.h"

class Hysterisis {
protected:
	float lastTime;
	bool lastValue, curValue;
	float delay;
public:
	Hysterisis() :
	delay(0),
	lastTime(0),
	lastValue(0),
	curValue(false) {
	}
	void setDelay(float delay) {
		this->delay = delay;
	}
	void set(bool value) {
		float curTime = ofGetElapsedTimef();
		if(value != curValue) {
			if(value != lastValue)
				lastTime = curTime;
			if(curTime - lastTime > delay)
				curValue = value;
		}
		lastValue = value;
	}
	bool get() {
		return curValue;
	}
};
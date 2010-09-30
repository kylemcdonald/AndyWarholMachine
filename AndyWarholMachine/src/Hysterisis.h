#pragma once

#include "ofMain.h"

class Hysterisis {
protected:
	float lastTime;
	bool lastValue, curValue;
	float delay;
	bool triggered, untriggered;
public:
	Hysterisis() :
	delay(0),
	lastTime(0),
	lastValue(0),
	triggered(false),
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
			if(curTime - lastTime > delay) {
				curValue = value;
				if(value)
					triggered = true;
				else
					untriggered = true;
			}
		}
		lastValue = value;
	}
	bool get() {
		return curValue;
	}
	bool wasTriggered() {
		if(triggered) {
			triggered = false;
			return true;
		}
		return false;
	}
	bool wasUntriggered() {
		if(untriggered) {
			untriggered = false;
			return true;
		}
		return false;
	}
	float length() {
		return ofGetElapsedTimef() - lastTime;
	}
};
#pragma once

#include "ofMain.h"

class DelayTimer {
protected:
	float period;
	float lastTime;
	int ticks;
public:
	DelayTimer() :
	period(0),
	lastTime(0),
	ticks(0) {
	};
	void setFramerate(float framerate) {
		period = 1 / framerate;
	}
	void setPeriod(float period) {
		this->period = period;
	}
	bool tick() {
		float curTime = ofGetElapsedTimef();
		int curTicks = (int) (curTime / period);
		int lastTicks = (int) (lastTime / period);
		lastTime = curTime;
		ticks += curTicks - lastTicks;
		if(ticks) {
			ticks--;
			return true;
		} else {
			return false;
		}
	}
};
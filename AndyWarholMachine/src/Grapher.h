#pragma once

#include "ofMain.h"

class Grapher : public ofBaseDraws {
protected:
	bool status;
	int width, height;
	float min, max;
	float minThreshold, maxThreshold;
	list<float> values; // should really be a circular buffer
public:
	Grapher();
	void setup(int width, int height, int min, int max);
	void setMinThreshold(float minThreshold);
	void setMaxThreshold(float maxThreshold);
	void addValue(float value);
	float getWidth();
	float getHeight();
	void draw(float x, float y);
	void draw(float x, float y, float width, float height);
	bool getStatus();
};
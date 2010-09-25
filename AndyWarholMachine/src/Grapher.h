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
	Grapher() :
		minThreshold(0),
		maxThreshold(0),
		status(false) {
	}
	void setup(int width, int height, int min, int max) {
		this->width = width;
		this->height = height;
		this->min = min;
		this->max = max;
	}
	void setMinThreshold(float minThreshold) {
		this->minThreshold = minThreshold;
	}
	void setMaxThreshold(float maxThreshold) {
		this->maxThreshold = maxThreshold;
	}
	void addValue(float value) {
		values.push_front(value);
		if(values.size() > width)
			values.pop_back();
		if(value < minThreshold)
			status = false;
		if(value > maxThreshold)
			status = true;
	}
	float getWidth() {
		return width;
	}
	float getHeight() {
		return height;
	}
	void draw(float x, float y) {
		draw(x, y, width, height);
	}
	void draw(float x, float y, float width, float height) {
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(x, y);
		
		ofFill();
		ofSetColor(0, 0, 0);
		ofRect(0, 0, width, height);
		
		ofNoFill();
		ofSetColor(255, 255, 255);
		ofRect(0, 0, width, height);
		glPushMatrix();
		glScalef(width / (this->width), 1, 0);
		glBegin(GL_LINE_STRIP);
		int i = 0;
		for(list<float>::iterator itr = values.begin(); itr != values.end(); itr++) {
			glVertex2f(this->width - i, ofMap(*itr, min, max, height, 0));
			i++;
		}
		glEnd();
		glPopMatrix();
		
		int minThresholdY = ofMap(minThreshold, min, max, height, 0);
		int maxThresholdY = ofMap(maxThreshold, min, max, height, 0);
		ofLine(0, minThresholdY, width, minThresholdY);
		ofLine(0, maxThresholdY, width, maxThresholdY);
		
		int top = 15;
		int bottom = height - 5;
		ofDrawBitmapString(ofToString((int) max), 5, top);
		ofDrawBitmapString(ofToString((int) values.front()), 5, (top + bottom) / 2);
		ofDrawBitmapString(ofToString((int) min), 5, bottom);
		
		ofPopMatrix();
		ofPopStyle();
	}
	bool getStatus() {
		return status;
	}
};
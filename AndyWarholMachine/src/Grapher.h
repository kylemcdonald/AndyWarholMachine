#pragma once

#include "ofMain.h"

class Grapher : public ofBaseDraws {
protected:
	int width, height;
	float min, max;
	list<float> values; // should really be a circular buffer
public:
	void setup(int width, int height, int min, int max) {
		this->width = width;
		this->height = height;
		this->min = min;
		this->max = max;
	}
	void addValue(float value) {
		values.push_front(ofMap(value, min, max, height, 0));
		if(values.size() > width)
			values.pop_back();
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
		glScalef(width / (this->width), height / (this->height), 0);
		glBegin(GL_LINE_STRIP);
		int i = 0;
		for(list<float>::iterator itr = values.begin(); itr != values.end(); itr++) {
			glVertex2f(this->width - i, *itr);
			i++;
		}
		glEnd();
		glPopMatrix();
		
		ofPopMatrix();
		ofPopStyle();
	}
};
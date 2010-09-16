#pragma once

#include "ofMain.h"

class Grapher {
protected:
	int width, height;
	float min, max;
	list<float> values;
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
	void draw(int x, int y) {
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(x, y);
		
		ofFill();
		ofSetColor(0, 0, 0);
		ofRect(0, 0, width, height);
		
		ofNoFill();
		ofSetColor(255, 255, 255);
		ofRect(0, 0, width, height);
		glBegin(GL_LINE_STRIP);
		int i = 0;
		for(list<float>::iterator itr = values.begin(); itr != values.end(); itr++) {
			glVertex2f(width - i, *itr);
			i++;
		}
		glEnd();
		
		ofPopMatrix();
		ofPopStyle();
	}
};
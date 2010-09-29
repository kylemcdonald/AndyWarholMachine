#pragma once

#include "ofMain.h"

class FloatImage : public ofBaseDraws {
protected:
	float* pixels;
	ofImage img;
	
	int size();
public:
	FloatImage();
	~FloatImage();
	void setup(int width, int height);
	void update();
	void draw(float x, float y);
	void draw(float x, float y, float width, float height);
	float getWidth();
	float getHeight();
	void lerp(float amt, ofBaseImage& img);
	void set(ofBaseImage& img);
	const float* getPixels();
};
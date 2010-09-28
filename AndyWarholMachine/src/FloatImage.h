#pragma once

#include "ofMain.h"

class FloatImage {
protected:
	float* pixels;
	ofImage img;
	
	int size();
public:
	FloatImage();
	~FloatImage();
	void setup(int width, int height);
	void update();
	void draw(int x, int y);
	void draw(int x, int y, int width, int height);
	void lerp(float amt, ofBaseImage& img);
	void set(ofBaseImage& img);
	const float* getPixels();
};
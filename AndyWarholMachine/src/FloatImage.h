#pragma once

#include "ofMain.h"

class FloatImage {
protected:
	float* pixels;
	ofImage img;
	
	int size() {
		return img.getWidth() * img.getHeight() * 3;
	}
public:
	FloatImage() :
		pixels(NULL) {
	}
	~FloatImage() {
		if(pixels != NULL)
			delete [] pixels;
	}
	void setup(int width, int height) {
		img.allocate(width, height, OF_IMAGE_COLOR);
		if(pixels != NULL)
			delete [] pixels;
		pixels = new float[size()];
	}
	void update() {
		int n = size();
		unsigned char* targetPixels = img.getPixels();
		for(int i = 0; i < n; i++)
			targetPixels[i] = (unsigned char) pixels[i];
		img.update();
	}
	void draw(int x, int y) {
		img.draw(x, y);
	}
	void draw(int x, int y, int width, int height) {
		img.draw(x, y, width, height);
	}
	void lerp(float amt, ofBaseImage& img) {
		int n = size();
		unsigned char* sourcePixels = img.getPixels();
		float invAmt = 1. - amt;
		for(int i = 0; i < n; i++)
			pixels[i] = invAmt * pixels[i] + amt * (float) sourcePixels[i];
	}
	void set(ofBaseImage& img) {
		lerp(1, img);
	}
	const float* getPixels() {
		return pixels;
	}
};
#pragma once

#include "ofMain.h"

class VideoBuffer {
protected:
	vector<ofImage> buffer;
public:
	VideoBuffer();
	void setup(int width, int height, int frames);
	void set(int i, ofBaseImage& img);
	virtual ofImage& get(int i);
};
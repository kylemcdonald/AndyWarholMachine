#pragma once

#include "ofMain.h"

class VideoBuffer {
protected:
	vector<ofImage> buffer;
public:
	VideoBuffer();
	void setup(int width, int height, int frames);
	ofImage& get(int i);
};
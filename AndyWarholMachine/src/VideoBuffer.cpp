#include "VideoBuffer.h"

VideoBuffer::VideoBuffer() {
}

void VideoBuffer::setup(int width, int height, int frames) {
	buffer.resize(frames);
	float start = ofGetElapsedTimef();
	for(int i = 0; i < frames; i++)
		buffer[i].allocate(width, height, OF_IMAGE_COLOR);
	float end = ofGetElapsedTimef();
	ofLog(OF_LOG_VERBOSE, "Allocated " + ofToString(frames) + " images (" + ofToString(end - start, 2) + " seconds).");
}

ofImage& VideoBuffer::get(int i) {
	return buffer[i];
}
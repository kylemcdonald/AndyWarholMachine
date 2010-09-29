#pragma once

#include "VideoBuffer.h"

class VideoDelay : public VideoBuffer {
protected:
	int writePosition, readPosition;
public:
	VideoDelay() :
	writePosition(0),
	readPosition(0) {
	}
	void add(ofBaseImage& img) {
		set(writePosition, img);
		//ofLog(OF_LOG_VERBOSE, "Current write position: " + ofToString(writePosition));
		writePosition++;
		if(writePosition == buffer.size())
			writePosition = 0;
	}
	ofImage& read(int offset) {
		int curPosition = (readPosition + buffer.size() - offset - 1) % buffer.size();
		//ofLog(OF_LOG_VERBOSE, "Current read position: " + ofToString(curPosition));
		ofImage& cur = get(curPosition);
		readPosition++;
		if(readPosition == buffer.size())
			readPosition = 0;
		return cur;
	}
};
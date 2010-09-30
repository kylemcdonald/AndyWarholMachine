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
		writePosition++;
		if(writePosition == buffer.size())
			writePosition = 0;
	}
	void setReadPosition(int readPosition) {
		this->readPosition = readPosition % buffer.size();
	}
	ofImage& read() {
		ofImage& cur = get(readPosition);
		readPosition++;
		if(readPosition == buffer.size())
			readPosition = 0;
		return cur;
	}
	float getReadPosition() {
		return (float) readPosition / buffer.size();
	}
	float getWritePosition() {
		return (float) writePosition / buffer.size();
	}
	void setReadFromWrite() {
		readPosition = writePosition;
	}
};
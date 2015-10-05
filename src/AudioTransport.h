//
// Created by ARJ on 10/4/15.
//

#ifndef BLIP_AUDIOTRANSPORT_H
#define BLIP_AUDIOTRANSPORT_H

#include <RtAudio.h>
#include <cstdint>
#include <functional>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "AudioTypes.h"

struct AudioPingPongBuffer {
	size_t bufferSize;
	StereoFrame *front;
	StereoFrame *back;

	std::mutex mut;
	std::condition_variable onSwap, onWrite;

	std::atomic_bool frontDirty {true};
	size_t sourceStreamStatus = 0; /// matches return convention from rtaudio callback

	AudioPingPongBuffer(size_t bufSize) :
			bufferSize(bufSize),
			front(new StereoFrame[bufSize]),
			back(new StereoFrame[bufSize]),
			mut(),
			onSwap(),
			onWrite()
	{}

	void swap();
};

void threaded_fill(AudioPingPongBuffer * buffer, StereoSourceFunction source);


class AudioTransport {
protected:
	RtAudio rtaudio;

	typedef std::list<AudioPingPongBuffer*> BufferList;
	BufferList sourceBuffers;

	unsigned int bufferSize = 1024;

public:

	AudioTransport();

	void start();

	int audioCallback(double *outputBuffer, double *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status);

	void addSource(StereoSourceFunction sampleFunc);
};

#endif //BLIP_AUDIOTRANSPORT_H

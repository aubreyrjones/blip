//
// Created by ARJ on 10/4/15.
//

#ifndef BLIP_AUDIOTRANSPORT_H
#define BLIP_AUDIOTRANSPORT_H

#include <RtAudio.h>
#include <cstdint>
#include <functional>
#include <list>

typedef std::function<size_t (double*, size_t)> SampleSourceFunction;


class AudioTransport {
protected:
	RtAudio rtaudio;
	std::list<SampleSourceFunction> allSources;

public:

	AudioTransport();

	void start();

	int audioCallback(double *outputBuffer, double *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status);

	void addSource(SampleSourceFunction sampleFunc) { allSources.push_back(sampleFunc); }
};

#endif //BLIP_AUDIOTRANSPORT_H

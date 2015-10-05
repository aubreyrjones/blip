//
// Created by ARJ on 10/4/15.
//

#include "AudioTransport.h"

int _rt_audio_callback(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData){
	AudioTransport *transport = reinterpret_cast<AudioTransport *>(userData);

	return transport->audioCallback(reinterpret_cast<double*>(outputBuffer),
									reinterpret_cast<double*>(inputBuffer),
									nFrames,
									streamTime,
									status);
}


AudioTransport::AudioTransport() :
	rtaudio()
{
	rtaudio.showWarnings(true);

	RtAudio::StreamParameters streamParams;
	streamParams.deviceId = rtaudio.getDefaultOutputDevice();
	streamParams.nChannels = 2;
	streamParams.firstChannel = 0;

	std::cout << streamParams.deviceId << std::endl;

	unsigned int bufferSize = 1024;

	rtaudio.openStream(&streamParams, nullptr, RTAUDIO_FLOAT64, 44100, &bufferSize, _rt_audio_callback, this, nullptr);
}

int AudioTransport::audioCallback(double *outputBuffer, double *, unsigned int nFrames, double streamTime, RtAudioStreamStatus status) {

	for (size_t i = 0; i < nFrames * 2; i++){ // * 2 for stereo
		outputBuffer[i] = 0;
	}

	for (auto s : allSources) {
		s(outputBuffer, nFrames);
	}

	return nFrames;
}

void AudioTransport::start() {
	rtaudio.startStream();
}

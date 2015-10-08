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


AudioTransport::AudioTransport(uint32_t outputSampleRate) :
	rtaudio()
{
	rtaudio.showWarnings(true);

	RtAudio::StreamParameters streamParams;
	streamParams.deviceId = rtaudio.getDefaultOutputDevice();
	streamParams.nChannels = 2;
	streamParams.firstChannel = 0;

	rtaudio.openStream(&streamParams, nullptr, RTAUDIO_FLOAT64, outputSampleRate, &bufferSize, _rt_audio_callback, this, nullptr);
}


int AudioTransport::audioCallback(double *outputBuffer, double *, unsigned int nFrames, double streamTime, RtAudioStreamStatus status) {
	StereoFrame *out = reinterpret_cast<StereoFrame*>(outputBuffer);

	for (size_t i = 0; i < nFrames; i++){
		out[i].zero();
	}

	std::vector<BufferList::iterator> toDelete;

	for (auto it = sourceBuffers.begin(); it != sourceBuffers.end(); ++it) {
		AudioPingPongBuffer *buf = *it;
		std::unique_lock<std::mutex> lock(buf->mut);

		while (buf->frontDirty) {
			buf->onWrite.wait(lock);
		}

		for (size_t i = 0; i < nFrames; i++) {
			out[i] += buf->front[i];
		}

		buf->swap();
		lock.unlock();
		buf->onSwap.notify_all();

		if (buf->sourceStreamStatus != 0){
			toDelete.push_back(it);
		}

	}

	if (!toDelete.empty()){
		for (auto it : toDelete) {
			delete *it;
			sourceBuffers.erase(it);
		}
	}

	if (sourceBuffers.empty()) {
		return 1;
	}

	return 0;
}

void AudioTransport::start() {
	rtaudio.startStream();
}

void AudioTransport::addSource(StereoSourceFunction sampleFunc) {

	AudioPingPongBuffer *buf = new AudioPingPongBuffer(bufferSize);

	sourceBuffers.push_back(buf);

	std::thread t(std::bind(threaded_fill, buf, sampleFunc));
	t.detach();
}


void threaded_fill(AudioPingPongBuffer *buffer, StereoSourceFunction source) {
	std::cout << "Source launched." << std::endl;

	while (buffer->sourceStreamStatus == 0) {
		std::unique_lock<std::mutex> lock(buffer->mut);

		if (buffer->frontDirty.load(std::memory_order_acquire)) {

			buffer->sourceStreamStatus =
					source(buffer->front, buffer->bufferSize);

			buffer->frontDirty.store(false, std::memory_order_release);

			lock.unlock();
			buffer->onWrite.notify_all();
		}
		else {
			buffer->onSwap.wait(lock);
		}
	}

	std::cout << "Source ended." << std::endl;
}


void AudioPingPongBuffer::swap() {
	std::swap(front, back);
	frontDirty = true;
}

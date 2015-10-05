//
// Created by ARJ on 9/24/15.
//

#ifndef BLIP_WAVREADER_H
#define BLIP_WAVREADER_H

#include <cstdint>
#include <functional>
#include <memory_mapped_file.hpp>
#include <limits>
#include "AudioTypes.h"

struct _WavChunkHeader {
	uint32_t chunkID;
	uint32_t chunkSize;
};

struct _WavRiffChunkDescriptor {
	_WavChunkHeader header;

	uint32_t format;
};

// we define the entire 40-byte header; if we get a shorter header
// we might alias some non-header data at the end. This isn't a big
// deal unless the audio data is pathologically short.
struct _WavFormatChunk {
	_WavChunkHeader header;

	uint16_t audioFormat;
	uint16_t nChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
	uint16_t extensionSize;
	uint16_t nValidBits;
	uint32_t speakerMask;
	uint16_t subFormat;
	uint8_t formatGUID[14];
};

template <typename S>
double convert_sample(S const& s);

template <>
inline double convert_sample(float const& s) {
	return (double) s;
}

template <>
inline double convert_sample(double const& s) {
	return s;
}

template <>
inline double convert_sample(uint8_t const& s) {
	return (s - 128.0) / 128.0;
}

template <>
inline double convert_sample(uint16_t const& s) {
	return (s - 32768.0) / 32768.0;
}

template <>
inline double convert_sample(int8_t const& s) {
	return (double) s / 128.0;
}

template <>
inline double convert_sample(int16_t const& s) {
	return (double) s / 32768.0;
}

class WavReader {
public:
protected:
	memory_mapped_file::read_only_mmf file;
	size_t frameCursor = 0;
	size_t sampleCursor;

	_WavFormatChunk format;
	size_t bytesPerChannel;
	size_t bytesPerFrame;
	size_t framesInFile;

	void advanceFrame();

	template <typename SAMPLE>
	SAMPLE s(size_t channel) {
		SAMPLE const* f = reinterpret_cast<SAMPLE const*>(file.data() + sampleCursor);
		return *(f + channel);
	}

	template <typename SAMPLE>
	void fill_frame(StereoFrame *out) {
		out->l = convert_sample(s<SAMPLE>(0));
		out->r = convert_sample(s<SAMPLE>(1));
	}

	template <typename SAMPLE>
	void fill_buffer(StereoFrame *out, size_t nFrames) {
		for (size_t i = 0; i < nFrames; i++) {
			fill_frame<SAMPLE>(out);
			advanceFrame();
			out++;
		}
	}

public:
	WavReader(std::string const& filepath) : file(filepath.c_str()) {}

	bool initialize(); /// initialize, and return false if the file can't be parsed

	size_t readAndConvertSamples(StereoFrame *buffer, size_t nFrames);
};

#endif //BLIP_WAVREADER_H

//
// Created by ARJ on 9/24/15.
//

#ifndef BLIP_WAVREADER_H
#define BLIP_WAVREADER_H

#include <cstdint>
#include <functional>

struct _WavChunkHeader {
	uint8_t chunkID [4];
	uint32_t chunkSize;
};

struct _WavRiffChunkDescriptor {
	uint8_t format_ [4];
};

struct _WavFormatChunk {
	uint32_t audioFormat;
	uint32_t nChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint32_t blockAlign;
	uint32_t bitsPerSample;
};


class WavReader {
public:
	typedef std::function<size_t (uint8_t*, size_t)> ReadFunction;
	typedef std::function<void (uint8_t*, size_t)> SinkFunction;

protected:
	ReadFunction _read;

public:
	WavReader(ReadFunction reader, SinkFunction ) : _read(reader) {}

	bool initialize(); /// initialize, and return false if the file can't be parsed
};

#endif //BLIP_WAVREADER_H

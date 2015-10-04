//
// Created by ARJ on 9/24/15.
//

#ifndef BLIP_WAVREADER_H
#define BLIP_WAVREADER_H

#include <cstdint>
#include <functional>
#include <memory_mapped_file.hpp>

struct _WavChunkHeader {
	uint32_t chunkID;
	uint32_t chunkSize;
};

struct _WavRiffChunkDescriptor {
	_WavChunkHeader header;

	uint32_t format;
};

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

class WavReader {
public:
protected:
	memory_mapped_file::read_only_mmf file;
	size_t sampleCursor;

	_WavFormatChunk format;
	size_t nChannels;
	size_t nSamplesInFile;

public:
	WavReader(std::string const& filepath) : file(filepath.c_str()) {}

	bool initialize(); /// initialize, and return false if the file can't be parsed
};

#endif //BLIP_WAVREADER_H

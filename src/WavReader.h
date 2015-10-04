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

	uint32_t format_;
};

struct _WavFormatChunk {
	_WavChunkHeader header;

	uint32_t audioFormat;
	uint32_t nChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint32_t blockAlign;
	uint32_t bitsPerSample;
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

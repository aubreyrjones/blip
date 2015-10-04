//
// Created by ARJ on 9/24/15.
//

#include "WavReader.h"
#include <iostream>

bool WavReader::initialize() {
	if (!file.is_open() || !file.data()) {
		return false;
	}

	if (file.mapped_size() < sizeof(_WavRiffChunkDescriptor)) {
		return false;
	}

	_WavRiffChunkDescriptor const* riffHeader = reinterpret_cast<_WavRiffChunkDescriptor const*>(file.data());

	if (riffHeader->header.chunkID != 'RIFF' || riffHeader->format_ != 'WAVE') {
		return false;
	}

	format = *reinterpret_cast<_WavFormatChunk const*>(file.data() + sizeof(_WavRiffChunkDescriptor));

	if (format.audioFormat != 1) {
		std::cout << "Only linear PCM supported right now." << std::endl;
	}

	if (format.bitsPerSample != 8){
		std::cout << "Only 8-bit PCM supported right now." << std::endl;
	}

	_WavChunkHeader chunkHeader = *reinterpret_cast<_WavChunkHeader*>(file.data());

	sampleCursor = sizeof(_WavRiffChunkDescriptor) + sizeof(_WavFormatChunk) + sizeof(_WavChunkHeader);

	return true;
}

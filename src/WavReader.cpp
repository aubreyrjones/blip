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

	if (riffHeader->header.chunkID != 'FFIR' || riffHeader->format != 'EVAW') {
		std::cout << "Not 'RIFF' or not 'WAVE'." << std::endl;
		return false;
	}

	_WavChunkHeader formatHeader = *reinterpret_cast<_WavChunkHeader const*>(&file.data()[sizeof(_WavRiffChunkDescriptor)]);

	if (formatHeader.chunkID != ' tmf') {
		std::cout << "Bad fmt_ chunk." << std::endl;
		return false;
	}


	format = *reinterpret_cast<_WavFormatChunk const*>(file.data() + sizeof(_WavRiffChunkDescriptor));

	if (format.audioFormat != 1 && format.audioFormat != 0xfffe) {
		std::cout << "Only linear PCM supported right now. Got: " << format.audioFormat << std::endl;
		return false;
	}

	if (format.audioFormat == 0xfffe && format.subFormat != 1) {
		std::cout << "Unknown extended wav format: " << format.subFormat << std::endl;
		return false;
	}

	sampleCursor =
			sizeof(_WavRiffChunkDescriptor) +
					sizeof(_WavFormatChunk) +
					sizeof(_WavChunkHeader);



	return true;
}

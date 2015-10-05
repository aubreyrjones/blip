//
// Created by ARJ on 9/24/15.
//

#include "WavReader.h"
#include <iostream>
#include <glm/glm.hpp>

bool WavReader::initialize() {
	if (!file.is_open() || !file.data()) {
		return false;
	}

	if (file.mapped_size() < sizeof(_WavRiffChunkDescriptor)) {
		return false;
	}

	size_t offset = 0;

	_WavRiffChunkDescriptor const* riffHeader = reinterpret_cast<_WavRiffChunkDescriptor const*>(file.data());

	if (riffHeader->header.chunkID != 'FFIR' || riffHeader->format != 'EVAW') {
		std::cout << "Not 'RIFF' or not 'WAVE'." << std::endl;
		return false;
	}

	offset += sizeof(_WavRiffChunkDescriptor);

	format = *reinterpret_cast<_WavFormatChunk const*>(file.data() + offset);

	if (format.header.chunkID != ' tmf') {
		std::cout << "Bad fmt_ chunk." << std::endl;
		return false;
	}

	if (format.audioFormat != 1 && format.audioFormat != 0xfffe) {
		std::cout << "Only linear PCM supported right now. Got: " << format.audioFormat << std::endl;
		return false;
	}

	if (format.audioFormat == 0xfffe) {
		if (format.header.chunkSize == 40 && format.subFormat != 1) {
			std::cout << "Unknown extended wav format: " << format.subFormat << std::endl;
			return false;
		}
		format.audioFormat = format.subFormat;
	}

	offset += sizeof(_WavFormatChunk);

	_WavChunkHeader dataHeader = *reinterpret_cast<_WavChunkHeader const*>(file.data() + offset);

	frameCursor =
			sizeof(_WavRiffChunkDescriptor) +
					sizeof(_WavFormatChunk) +
					sizeof(_WavChunkHeader);

	bytesPerChannel = (size_t) glm::ceil(format.bitsPerSample / 8.0f);
	bytesPerFrame = format.nChannels * bytesPerChannel;
	framesInFile = dataHeader.chunkSize / bytesPerFrame;

	return true;
}

size_t WavReader::readAndConvertSamples(double *buffer, size_t nFrames) {
	if (frameCursor + nFrames > framesInFile) {
		nFrames = framesInFile - frameCursor;
	}

	if (format.audioFormat == 1){
		if (bytesPerChannel == 1){
			fill_buffer<uint8_t>(buffer, nFrames);
		}
		else if (bytesPerChannel == 2){
			fill_buffer<int16_t>(buffer, nFrames);
		}
	}


	return nFrames;
}

void WavReader::advanceFrame() {
	frameCursor += bytesPerFrame;
}

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

	offset += format.header.chunkSize + sizeof(_WavChunkHeader);

	_WavChunkHeader dataHeader = *reinterpret_cast<_WavChunkHeader const*>(file.data() + offset);

	offset += sizeof(_WavChunkHeader);

	sampleCursor = offset;
	std::cout << "data offset: " << frameCursor << std::endl;

	bytesPerChannel = (size_t) glm::ceil(format.bitsPerSample / 8.0f);
	bytesPerFrame = format.nChannels * bytesPerChannel;
	framesInFile = dataHeader.chunkSize / bytesPerFrame;

	std::cout << "nChannels: " << format.nChannels << std::endl;
	std::cout << "byte alignment: " << format.blockAlign << std::endl;
	std::cout << "Sample rate: " << format.sampleRate << std::endl;
	std::cout << "bpc: " << bytesPerChannel << std::endl;
	std::cout << "bpf: " << bytesPerFrame << std::endl;
	std::cout << "filesize (bytes): " << file.file_size() << std::endl;

	return true;
}

size_t WavReader::readAndConvertSamples(StereoFrame *buffer, size_t nFrames) {
	size_t nFramesToSample = nFrames;

	if (frameCursor + nFrames > file.file_size()) {
		nFramesToSample = framesInFile - frameCursor;
	}

	if (!nFramesToSample) return 2;

	if (format.audioFormat == 1){
		if (bytesPerChannel == 1){
			fill_buffer<uint8_t>(buffer, nFramesToSample);
		}
		else if (bytesPerChannel == 2){
			fill_buffer<int16_t>(buffer, nFramesToSample);
		}
	}

	if (nFramesToSample < nFrames) {
		for (StereoFrame *f = buffer + nFramesToSample; f < buffer + nFramesToSample; f++) {
			f->l = 0;
			f->r = 0;
		}
		return 1;
	}

	return 0;
}

void WavReader::advanceFrame() {
	frameCursor++;
	sampleCursor += bytesPerFrame;
}

//
// Created by ARJ on 10/7/15.
//

#include "Resampler.h"

StereoResampler::StereoResampler(StereoSourceFunction source, uint32_t sampleRateIn, uint32_t sampleRateOut, size_t maxNFrames) :
		source(source),
		lResample(sampleRateIn, sampleRateOut, maxNFrames),
		rResample(sampleRateIn, sampleRateOut, maxNFrames),
		workBuffer(new double[maxNFrames]),
		readBuffer(new StereoFrame[maxNFrames])
{

}

StereoResampler::~StereoResampler() {
	delete workBuffer;
}

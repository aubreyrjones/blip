//
// Created by ARJ on 10/7/15.
//

#ifndef BLIP_RESAMPLER_H
#define BLIP_RESAMPLER_H

#include "AudioTypes.h"
#include <r8bbase.h>
#include <CDSPResampler.h>

class StereoResampler {
	StereoSourceFunction source; ///< Where we're getting audio data.
	r8b::CDSPResampler24 lResample, rResample; ///<
	double *workBuffer;
	StereoFrame *readBuffer;

public:
	StereoResampler(StereoSourceFunction source, uint32_t sampleRateIn, uint32_t sampleRateOut, size_t maxNFrames);
	StereoResampler(StereoResampler & o) = delete;
	virtual ~StereoResampler();
};

#endif //BLIP_RESAMPLER_H

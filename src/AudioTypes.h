//
// Created by ARJ on 10/4/15.
//

#ifndef BLIP_AUDIOTYPES_H
#define BLIP_AUDIOTYPES_H

#include <functional>

struct StereoFrame {
	double l, r;

	StereoFrame() {}
	StereoFrame(double l, double r) : l(l), r(r) {}

	void zero() {
		l = r = 0;
	}

	StereoFrame operator+(StereoFrame const& o) {
		return StereoFrame(l + o.l, r + o.r);
	}

	StereoFrame& operator+=(StereoFrame const& o) {
		l += o.l;
		r += o.r;

		return *this;
	}
};

typedef std::function<size_t (StereoFrame*, size_t)> StereoSourceFunction;


#endif //BLIP_AUDIOTYPES_H

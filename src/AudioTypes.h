//
// Created by ARJ on 10/4/15.
//

#ifndef BLIP_AUDIOTYPES_H
#define BLIP_AUDIOTYPES_H

#include <functional>

struct StereoFrame {
	double l, r;
};

typedef std::function<size_t (StereoFrame*, size_t)> StereoSourceFunction;


#endif //BLIP_AUDIOTYPES_H

#pragma once
#include "animation.h"

class LinearAnimation_t : public Animation_t {
public:
	LinearAnimation_t(float min_val, float max_val, float duration);
protected:
	bool _step(float time) override;

	float _duration;
	float _min_val;
	float _max_val;
};
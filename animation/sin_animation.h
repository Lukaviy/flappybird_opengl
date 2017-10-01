#pragma once
#include "animation.h"

class SinAnimation_t : public Animation_t {
public:
	SinAnimation_t(float min_val, float max_val, float frequency, float phase, float duration = 0);
protected:
	void _step(float time) override;

	float _min_val;
	float _max_val;
	float _frequency;
	float _phase;
	float _duration;
};

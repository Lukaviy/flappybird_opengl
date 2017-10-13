#pragma once
#include "animation.h"

class TimerAnimation_t : public Animation_t {
public:
	TimerAnimation_t(float duration);
protected:
	void _step(float time) override;
	float _duration;
};

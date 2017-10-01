#pragma once
#include "animation.h"

class StarterAnimation_t : public Animation_t {
protected:
	void _step(float time) override;
};

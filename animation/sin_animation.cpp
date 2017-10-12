#include "sin_animation.h"
#include <corecrt_math_defines.h>

SinAnimation_t::SinAnimation_t(float min_val, float max_val, float frequency, float phase, float duration) :
	_min_val(min_val), _max_val(max_val), _frequency(frequency), _phase(phase), _duration(duration) {}

void SinAnimation_t::_step(float time) {
	if (_duration > 0 && time >= _duration) {
		_state = PLAYED;
	} else {
		_val = (sin(time * _frequency + _phase - M_PI_2) + 1.f) / 2.f * (_max_val - _min_val) + _min_val;
	}
}

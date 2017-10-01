#include "sin_animation.h"

SinAnimation_t::SinAnimation_t(float min_val, float max_val, float frequency, float phase, float duration) :
	_min_val(min_val), _max_val(max_val), _frequency(frequency), _phase(phase), _duration(duration) {}

bool SinAnimation_t::_step(float time) {
	if (_duration > 0 && time >= _start_time + _duration) {
		return true;
	}
	_val = (sin((time - _start_time) * _frequency + _phase) + 1.f) / 2.f * (_max_val - _min_val) + _min_val;
	return false;
}

#include "linear_animation.h"

LinearAnimation_t::LinearAnimation_t(float min_val, float max_val, float duration)
	: _duration(duration), _min_val(min_val), _max_val(max_val) {}

bool LinearAnimation_t::_step(float time) {
	if (_start_time < 0) {
		_val = _min_val;
		return false;
	}
	if (time > _start_time + _duration) {
		_val = _max_val;
		return true;
	}
	_val = (time - _start_time) / _duration * (_max_val - _min_val) + _min_val;
	return false;
}

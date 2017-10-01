#include "linear_animation.h"

LinearAnimation_t::LinearAnimation_t(float min_val, float max_val, float duration)
	: _duration(duration), _min_val(min_val), _max_val(max_val) {}

void LinearAnimation_t::_step(float time) {
	if (time > _duration) {
		_val = _max_val;
		_state = PLAYED;
	} else {
		_val = time > _duration ? _val = _max_val : time / _duration * (_max_val - _min_val) + _min_val;
	}
}

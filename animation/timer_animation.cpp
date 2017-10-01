#include "timer_animation.h"

TimerAnimation_t::TimerAnimation_t(float duration) : _duration(duration) {}

void TimerAnimation_t::_step(float time) {
	if (time >= _duration) {
		_state = PLAYED;
	}
}

#include "timer_animation.h"

TimerAnimation_t::TimerAnimation_t(float duration) : _duration(duration) {}

bool TimerAnimation_t::_step(float time) {
	return time >= _duration;
}

#include "animation.h"
#include "animator.h"

Animation_t::Animation_t() : _val(0), _state(WAIT) {}

float Animation_t::val() const {
	return _val;
}

Animation_t::State_t Animation_t::state() const {
	return _state;
}

void Animation_t::on_start(std::function<void()> callback) {
	_on_start_callbacks.push_back(callback);
}

void Animation_t::on_stop(std::function<void()> callback) {
	_on_stop_callbacks.push_back(callback);
}

void Animation_t::start() {
	_state = PLAYING;
	for (auto callback : _on_start_callbacks) {
		callback();
	}
}

void Animation_t::stop() {
	_state = PLAYED;
	for (auto callback : _on_stop_callbacks) {
		callback();
	}
}

void Animation_t::_reset() {
	_state = WAIT;
	_val = 0;
}

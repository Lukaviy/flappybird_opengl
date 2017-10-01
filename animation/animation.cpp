#include "animation.h"
#include "animator.h"

Animation_t::Animation_t() : _val(0), _playing(false) {}

Animation_t::~Animation_t() {
	for (auto animation : _start_after_animations) {
		animation->_start();
	}
	for (auto pointer : _pointers) {
		pointer->_alive = false;
		pointer->_animation = nullptr;
		pointer->_val = _val;
	}
	for (auto callback : _on_stop_callbacks) {
		callback();
	}
}

void Animation_t::_start() {
	_playing = true;
	for (auto callback : _on_start_callbacks) {
		callback();
	}
	for (auto animation : _start_with_animations) {
		animation->_start();
	}
}

void Animation_t::_start_after(Animation_t* animation) {
	animation->_start_after_animations.push_back(this);
}

void Animation_t::_start_with(Animation_t* animation) {
	animation->_start_with_animations.push_back(this);
}

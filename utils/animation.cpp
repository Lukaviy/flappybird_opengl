#include "animation.h"
#include "animator.h"

Animation_t::Animation_t() : _val(0), _playing(false) {}

Animation_t::~Animation_t() {
	for (auto animation : _child_animations) {
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
}

void Animation_t::_start_after(Animation_t* animation) {
	animation->_child_animations.push_back(this);
}

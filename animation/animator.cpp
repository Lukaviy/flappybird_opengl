#include "animator.h"

Animator_t::Animator_t() : _elapsed_time(0) {}

Animator_t::~Animator_t() {
	for (auto container : _animations) {
		delete container.animation;
	}
}

void Animator_t::step(float dtm) {
	_elapsed_time += dtm;
	for (auto i = _animations.begin(); i != _animations.end(); ++i) {
		if (i->animation->_playing && i->animation->_step(_elapsed_time - i->start_time)) {
			delete i++->animation;
		}
		if (i == _animations.end()) {
			break;
		}
	}
}

float Animator_t::elapsed_time() const {
	return _elapsed_time;
}

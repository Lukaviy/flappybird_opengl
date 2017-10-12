#include "animator.h"

Animator_t::Animator_t() : _elapsed_time(0) {}

Animator_t::~Animator_t() {
	for (auto container : _paused_animations) {
		delete container.animation;
	}
	for (auto container : _playing_animations) {
		delete container.animation;
	}
}

void Animator_t::step(float dtm) {
	_elapsed_time += dtm;
	for (auto i = _playing_animations.begin(); i != _playing_animations.end(); ++i) {
		if (i->animation->state() == Animation_t::PLAYED) {
			i++->animation->stop();
		} else {
			i->animation->step(_elapsed_time - i->start_time);
		}
		if (i == _playing_animations.end()) {
			break;
		}
	}
}

void Animator_t::reset() {
	_paused_animations.splice(_paused_animations.end(), _playing_animations);
	for (auto container : _paused_animations) {
		container.animation->reset();
	}
}

void Animator_t::start_after(const AnimPointer_t& before, const AnimPointer_t& after) {
	before._animation->on_stop([after]()
	{
		after.start();
	});
}

void Animator_t::start_with(const AnimPointer_t& before, const AnimPointer_t& after) {
	before._animation->on_start([after]()
	{
		after.start();
	});
}

float Animator_t::elapsed_time() const {
	return _elapsed_time;
}

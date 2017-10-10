#include "animation_pointer.h"
#include "animation.h"
#include "animator.h"

AnimPointer_t::AnimPointer_t(Animation_t* animation, Animator_t* animator) :
	_animation(animation), _animator(animator) {}

AnimPointer_t::AnimPointer_t() : _animation(nullptr), _animator(nullptr) {}

float AnimPointer_t::val() const {
	return _animation->val();
}

float AnimPointer_t::val_or(float v) const {
	return alive() ? val() : v;
}

void AnimPointer_t::start() const {
	_animation->start();
}

const AnimPointer_t& AnimPointer_t::start_after(const AnimPointer_t& pointer) const {
	if (_animator == pointer._animator) {
		_animator->start_after(pointer, *this);
	}
	return *this;
}

const AnimPointer_t& AnimPointer_t::start_with(const AnimPointer_t& pointer) const {
	if (_animator == pointer._animator) {
		_animator->start_with(pointer, *this);
	}
	return *this;
}

bool AnimPointer_t::playing() const {
	return _animation && _animation->state() == Animation_t::PLAYING;
}

bool AnimPointer_t::alive() const {
	return _animation != nullptr;
}

const AnimPointer_t& AnimPointer_t::on_start(std::function<void()> callback) const {
	_animation->on_start(callback);
	return *this;
}

const AnimPointer_t& AnimPointer_t::on_stop(std::function<void()> callback) const {
	_animation->on_stop(callback);
	return *this;
}

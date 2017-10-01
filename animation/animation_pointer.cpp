#include "animation_pointer.h"
#include "animation.h"

AnimPointer_t::AnimPointer_t(Animation_t* animation) :
	_val(0), _alive(true), _animation(animation)
{
	_iter_to_this = _animation->_pointers.insert(_animation->_pointers.end(), this);
}


AnimPointer_t::AnimPointer_t() : _val(0), _alive(false), _animation(nullptr) {}

AnimPointer_t::AnimPointer_t(const AnimPointer_t& pointer) :
	_val(pointer._val), _alive(pointer._alive), _animation(pointer._animation)
{
	_iter_to_this = pointer._animation->_pointers.insert(pointer._animation->_pointers.end(), this);
}

AnimPointer_t& AnimPointer_t::operator=(const AnimPointer_t& pointer) {
	if (pointer._alive) {
		_iter_to_this = pointer._animation->_pointers.insert(pointer._animation->_pointers.end(), this);
	}
	_val = 0;
	_alive = pointer._alive;
	_animation = pointer._animation;
	return *this;
}

AnimPointer_t::~AnimPointer_t() {
	if (_alive) {
		_animation->_pointers.erase(_iter_to_this);
	}
}

float AnimPointer_t::val() {
	if (_alive) {
		_val = _animation->_val;
	}
	return _val;
}

float AnimPointer_t::cached_val() const {
	return _val;
}

void AnimPointer_t::start() const {
	_animation->_start();
}

const AnimPointer_t& AnimPointer_t::start_after(const AnimPointer_t& pointer) const {
	_animation->_start_after(pointer._animation);
	return *this;
}

const AnimPointer_t& AnimPointer_t::start_with(const AnimPointer_t& pointer) const {
	_animation->_start_with(pointer._animation);
	return *this;
}

void AnimPointer_t::stop() {
	delete _animation;
	_animation = nullptr;
	_alive = false;
}

bool AnimPointer_t::playing() const {
	return _alive && _animation->_playing;
}

const AnimPointer_t& AnimPointer_t::on_start(std::function<void()> callback) const {
	if (_alive) {
		_animation->_on_start_callbacks.push_back(callback);
	}
	return *this;
}

const AnimPointer_t& AnimPointer_t::on_stop(std::function<void()> callback) const {
	if (_alive) {
		_animation->_on_stop_callbacks.push_back(callback);
	}
	return *this;
}
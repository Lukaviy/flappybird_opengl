#include "animation.h"
#include <ctime>

std::list<Animation_t*> Animation_t::_animations;
std::chrono::system_clock::time_point Animation_t::_app_start_time = std::chrono::system_clock::now();

Animation_t::Animation_t() : _playing(false) {
	_iter_to_list = _animations.insert(_animations.end(), this);
}

Animation_t::~Animation_t() {
	_animations.erase(_iter_to_list);
	for (auto animation : _child_animations) {
		animation->_start();
	}
	for (auto pointer : _pointers) {
		pointer->_alive = false;
		pointer->_animation = nullptr;
	}
	for (auto callback : _on_stop_callbacks) {
		callback();
	}
}

Animation_t::Pointer_t::Pointer_t() : _val(0), _alive(false), _animation(nullptr) {}

void Animation_t::_start() {
	_start_time = current_time();
	_playing = true;
	for (auto callback : _on_start_callbacks) {
		callback();
	}
}

void Animation_t::_start_after(Animation_t* animation) {
	animation->_child_animations.push_back(this);
}

float Animation_t::current_time() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _app_start_time).count() / 1000.f;
}

void Animation_t::step() {
	float elapsed_time = current_time();
	for (auto i = _animations.begin(); i != _animations.end(); ++i) {
		if ((*i)->_playing && (*i)->_step(elapsed_time)) {
			auto next_iter = i;
			++next_iter;
			delete *i;
			i = next_iter;
		}
	}
}

Animation_t::Pointer_t::Pointer_t(Animation_t* animation) : _val(0), _alive(true), _animation(animation) {
	_iter_to_this = _animation->_pointers.insert(_animation->_pointers.end(), this);
}

Animation_t::Pointer_t::Pointer_t(const Pointer_t& pointer) : _val(pointer._val), _alive(pointer._alive), _animation(pointer._animation) {
	_iter_to_this = pointer._animation->_pointers.insert(pointer._animation->_pointers.end(), this);
}

Animation_t::Pointer_t& Animation_t::Pointer_t::operator=(const Pointer_t& pointer) {
	_iter_to_this = pointer._animation->_pointers.insert(pointer._animation->_pointers.end(), this);
	_val = 0;
	_alive = pointer._alive;
	_animation = pointer._animation;
	return *this;
}

Animation_t::Pointer_t::~Pointer_t() {
	if (_alive) {
		_animation->_pointers.erase(_iter_to_this);
	}
}

float Animation_t::Pointer_t::val() {
	if (_alive) {
		_val = _animation->_val;
	}
	return _val;
}

float Animation_t::Pointer_t::cached_val() const {
	return _val;
}

void Animation_t::Pointer_t::start() const {
	_animation->_start();
}

const Animation_t::Pointer_t& Animation_t::Pointer_t::start_after(const Pointer_t& pointer) const {
	_animation->_start_after(pointer._animation);
	return *this;
}

void Animation_t::Pointer_t::stop() {
	delete _animation;
	_animation = nullptr;
	_alive = false;
}

bool Animation_t::Pointer_t::playing() const {
	return _alive && _animation->_playing;
}

void Animation_t::Pointer_t::on_start(std::function<void()> callback) const {
	if (_alive) {
		_animation->_on_start_callbacks.push_back(callback);
	}
}

void Animation_t::Pointer_t::on_stop(std::function<void()> callback) const {
	if (_alive) {
		_animation->_on_stop_callbacks.push_back(callback);
	}
}

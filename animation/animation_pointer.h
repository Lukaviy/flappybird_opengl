#pragma once
#include "animation.h"
#include <list>
#include <functional>

class Animator_t;

class AnimPointer_t {
public:
	AnimPointer_t();
	float val() const;
	float val_or(float val) const;
	void start() const;
	bool playing() const;
	bool alive() const;
	const AnimPointer_t& start_after(const AnimPointer_t& pointer) const;
	const AnimPointer_t& start_with(const AnimPointer_t& pointer) const;
	const AnimPointer_t& on_start(std::function<void()> callback) const;
	const AnimPointer_t& on_stop(std::function<void()> callback) const;
protected:
	friend Animator_t;

	AnimPointer_t(Animation_t* animation, Animator_t* animator);

	Animator_t* _animator;
	Animation_t* _animation;
};

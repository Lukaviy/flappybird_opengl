#pragma once
#include <list>
#include <functional>

class Animation_t;
class Animator_t;

class AnimPointer_t {
public:
	AnimPointer_t();
	AnimPointer_t(const AnimPointer_t& pointer);
	AnimPointer_t& operator=(const AnimPointer_t& pointer);
	~AnimPointer_t();
	float val();
	float cached_val() const;
	void start() const;
	const AnimPointer_t& start_after(const AnimPointer_t& pointer) const;
	void stop();
	bool playing() const;
	void on_start(std::function<void()> callback) const;
	void on_stop(std::function<void()> callback) const;
protected:
	AnimPointer_t(Animation_t* animation);

	float _val;
	bool _alive;
	Animation_t* _animation;
	friend Animation_t;
	friend Animator_t;
	std::list<AnimPointer_t*>::iterator _iter_to_this;
};

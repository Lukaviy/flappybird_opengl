#pragma once
#include <list>
#include <vector>
#include <functional>

class AnimPointer_t;
class Animator_t;

class Animation_t {
public:
	Animation_t();
	virtual ~Animation_t();

	friend AnimPointer_t;
	friend Animator_t;
protected:
	float _val;
	bool _playing;

	std::vector<Animation_t*> _start_after_animations;
	std::vector<Animation_t*> _start_with_animations;
	std::list<AnimPointer_t*> _pointers;

	void _start();
	void _start_after(Animation_t* animation);
	void _start_with(Animation_t* animation);
	virtual bool _step(float time) = 0;

	std::list<std::function<void()>> _on_start_callbacks;
	std::list<std::function<void()>> _on_stop_callbacks;
};

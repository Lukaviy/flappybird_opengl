#pragma once
#include <list>
#include "animation.h"
#include "animation_pointer.h"

class Animator_t {
public:
	Animator_t();
	~Animator_t();
	template<class AnimationType, typename... Args>
	AnimPointer_t make(Args... args);
	void step(float dtm);
	float elapsed_time() const;
private:
	float _elapsed_time;
	struct AnimationContainer_t {
		Animation_t* animation;
		float start_time;
	};
	std::list<AnimationContainer_t> _animations;
};

template <class AnimationType, typename... Args>
AnimPointer_t Animator_t::make(Args... args) {
	auto animation = new AnimationType(args...);
	auto iter_to_list = _animations.insert(_animations.end(), AnimationContainer_t{ animation, 0.f });

	animation->_on_start_callbacks.push_back([this, iter_to_list]()
	{
		iter_to_list->start_time = _elapsed_time;
	});

	animation->_on_stop_callbacks.push_back([this, iter_to_list]()
	{
		_animations.erase(iter_to_list);
	});

	return AnimPointer_t(animation);
}

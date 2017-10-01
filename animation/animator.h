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
	void reset();
	static void start_after(const AnimPointer_t& before, const AnimPointer_t& after);
	static void start_with(const AnimPointer_t& before, const AnimPointer_t& after);
	float elapsed_time() const;
private:
	float _elapsed_time;
	struct AnimationContainer_t {
		Animation_t* animation;
		float start_time;
	};
	std::list<AnimationContainer_t> _playing_animations;
	std::list<AnimationContainer_t> _paused_animations;
};

template <class AnimationType, typename... Args>
AnimPointer_t Animator_t::make(Args... args) {
	auto animation = new AnimationType(args...);
	auto iter_to_list = _paused_animations.insert(_paused_animations.end(), AnimationContainer_t{ animation, 0.f });

	animation->on_start([this, iter_to_list]()
	{
		iter_to_list->start_time = _elapsed_time;
		_playing_animations.splice(_playing_animations.end(), _paused_animations, iter_to_list);
	});

	animation->on_stop([this, iter_to_list]()
	{
		_paused_animations.splice(_paused_animations.end(), _playing_animations, iter_to_list);
	});

	return AnimPointer_t(animation, this);
}

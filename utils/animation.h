#pragma once
#include <list>
#include <vector>
#include <functional>
#include <chrono>

class Animation_t {
public:
	Animation_t();

	virtual ~Animation_t();
	class Pointer_t {
	public:
		Pointer_t();
		Pointer_t(const Pointer_t& pointer);
		Pointer_t& operator=(const Pointer_t& pointer);
		~Pointer_t();
		float val();
		float cached_val() const;
		void start() const;
		const Pointer_t& start_after(const Pointer_t& pointer) const;
		void stop();
		bool playing() const;
		void on_start(std::function<void()> callback) const;
		void on_stop(std::function<void()> callback) const;
	protected:
		Pointer_t(Animation_t* animation);

		float _val;
		bool _alive;
		Animation_t* _animation;
		friend Animation_t;
		std::list<Pointer_t*>::iterator _iter_to_this;
	};

	template<class AnimationType, typename... Args>
	static Pointer_t make(Args... args);

	static void step();
	
protected:
	float _start_time;
	float _val;
	bool _playing;
	std::vector<Animation_t*> _child_animations;
	std::list<Animation_t*>::iterator _iter_to_list;
	std::list<Pointer_t*> _pointers;

	void _start();
	void _start_after(Animation_t* animation);
	virtual bool _step(float time) = 0;

	std::list<std::function<void()>> _on_start_callbacks;
	std::list<std::function<void()>> _on_stop_callbacks;

	static std::list<Animation_t*> _animations;
	static float current_time();
	static std::chrono::system_clock::time_point _app_start_time;
};

template <class AnimationType, typename ... Args>
Animation_t::Pointer_t Animation_t::make(Args... args) {
	auto animation = new AnimationType(args...);
	return Pointer_t(animation);
}

#pragma once
#include <list>
#include <functional>

class Animator_t;

class Animation_t {
public:
	Animation_t();
	virtual ~Animation_t();

	float val() const;

	enum State_t {
		WAIT,
		PLAYING,
		PLAYED
	} state() const;

	void on_start(std::function<void()>);
	void on_step(std::function<void(float val)>);
	void on_stop(std::function<void()>);
	void on_reset(std::function<void()>);
	void on_destroy(std::function<void()>);

	void start();
	void stop();

	friend Animator_t;
protected:
	float _val;
	
	State_t _state;

	void step(float time);
	virtual void _step(float time) = 0;
	void reset();
	virtual void _reset();

	std::list<std::function<void()>> _on_start_callbacks;
	std::list<std::function<void(float val)>> _on_step_callbacks;
	std::list<std::function<void()>> _on_stop_callbacks;
	std::list<std::function<void()>> _on_reset_callbacks;
	std::list<std::function<void()>> _on_destroy_callbacks;
};

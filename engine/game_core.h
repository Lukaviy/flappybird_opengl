#pragma once
#include <cmath>
#include <vector>
#include "ring_buffer.h"

class GameCore_t {
public:
	GameCore_t(size_t map_size, float x_velocity, float y_accel, float y_flap_accel, float tube_dist, float tube_width, float hole_size, float start_pos, float bird_height, float bird_width);

	struct Vec_t {
		float x;
		float y;
	};

	struct MapElement_t {
		float x_pos;
		float y_pos;
		float hole_size;
	};

	enum State_t {
		PLAYING,
		DEAD,
	};

	void step(float dtime, bool flap);
	void reset();

	Vec_t get_bird_pos() const;
	Vec_t get_bird_velocity() const;
	int get_current_ceil() const;
	State_t get_state() const;
	void freeze_x_pos(bool freeze);

	const RingBuffer_t<MapElement_t>& get_map() const;
private:
	float _x_pos;
	float _y_pos;
	float _x_velocity;
	float _y_velocity;
	float _y_accel;
	float _y_flap_velocity;
	float _tube_dist;
	float _tube_width;
	float _bird_height;
	float _bird_width; 
	float _hole_size;
	float _start_pos;
	State_t _state;
	RingBuffer_t<MapElement_t> _map;
	RingBuffer_t<MapElement_t>::iterator _curr_cell;
	bool _freeze_x_pos;

	MapElement_t gen_map_element(float x_pos) const;
};

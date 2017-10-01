#pragma once
#include <cmath>
#include <vector>

class GameCore_t {
public:
	GameCore_t(size_t map_size, float x_velocity, float y_accel, float y_flap_accel, float tube_dist, float tube_width, float hole_size, float start_pos, float bird_height, float bird_width);

	struct Vec_t {
		float x;
		float y;
	};

	struct MapElement_t {
		float y_pos; 
		float hole_size;
		float x_pos;
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

	const std::vector<MapElement_t>& get_map() const;
private:
	float _x_pos;
	float _y_pos;
	float _x_velocity;
	float _y_velocity;
	float _y_accel;
	float _y_flap_accel;
	float _tube_dist;
	float _tube_width;
	int _curr_cell;
	float _bird_height;
	float _bird_width; 
	float _hole_size;
	float _start_pos;
	State_t _state;
	std::vector<MapElement_t> _map;
};

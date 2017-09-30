#pragma once
#include <cmath>

template<int MAP_SIZE>
class GameCore_t {
public:
	GameCore_t(float x_velocity, float y_accel, float y_flap_accel, float tube_dist, float tube_width, float hole_size, float start_pos, float bird_height, float bird_width);

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

	const MapElement_t* get_map();
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
	MapElement_t _map[MAP_SIZE];
};

template <int MAP_SIZE>
GameCore_t<MAP_SIZE>::GameCore_t(float x_velocity, float y_accel, float y_flap_accel, float tube_dist, float tube_width, float hole_size, float start_pos, float bird_heiht, float bird_width)
	: _x_pos(start_pos), _y_pos(0.5), _x_velocity(x_velocity), _y_velocity(0), _y_accel(y_accel), _y_flap_accel(y_flap_accel), _tube_dist(tube_dist),
	_tube_width(tube_width), _curr_cell(0), _bird_height(bird_heiht), _bird_width(bird_width), _hole_size(hole_size), _state(PLAYING), _start_pos(start_pos)
{
	for (int i = 0; i < MAP_SIZE; i++) {
		_map[i].y_pos = -0.6f + i * 1.f / MAP_SIZE * 1.8f;
		_map[i].hole_size = hole_size;
		_map[i].x_pos = i * (tube_width + tube_dist);
	}
}

template <int MAP_SIZE>
void GameCore_t<MAP_SIZE>::step(float dtime, bool flap) {
	if (_state == DEAD) {
		if (_y_pos >= -1) {
			_y_velocity += _y_accel * dtime;
			_y_pos += _y_velocity * dtime;
		}
		return;
	}

	if (_map[_curr_cell % MAP_SIZE].x_pos + _tube_width / 2 <= _x_pos - _bird_width / 2) {
		_map[(_curr_cell + MAP_SIZE - 1) % MAP_SIZE].x_pos = _map[(_curr_cell + MAP_SIZE - 2) % MAP_SIZE].x_pos + _tube_width + _tube_dist;
		_map[(_curr_cell + MAP_SIZE - 1) % MAP_SIZE].y_pos = -0.7f + (rand() % 160) / 100.f;
		_map[(_curr_cell + MAP_SIZE - 1) % MAP_SIZE].hole_size = _hole_size + (rand() % 50) / 100.f;
		++_curr_cell;
	}
	auto hole = _map[_curr_cell % MAP_SIZE];
	if ((_x_pos + _bird_width / 2 >= hole.x_pos - _tube_width / 2 &&
		(_y_pos + _bird_height / 2 >= hole.y_pos + hole.hole_size / 2 || _y_pos - _bird_height / 2 <= hole.y_pos - hole.hole_size / 2) ||
		_y_pos <= -1 || _y_pos >= 1))
	{
		_state = DEAD;
	}
		_y_velocity += (_y_accel + _y_flap_accel * flap) * dtime;
		_x_pos += _x_velocity * dtime;
		_y_pos += _y_velocity * dtime;
}


template <int MAP_SIZE>
void GameCore_t<MAP_SIZE>::reset() {
	_x_pos = _start_pos;
	_y_pos = 0.5;
	_y_velocity = 0;
	_state = PLAYING;
	_curr_cell = 0;
	for (int i = 0; i < MAP_SIZE; i++) {
		_map[i].x_pos = i * (_tube_width + _tube_dist);
	}
}

template <int MAP_SIZE>
typename GameCore_t<MAP_SIZE>::Vec_t GameCore_t<MAP_SIZE>::get_bird_pos() const {
	return Vec_t{ _x_pos, _y_pos };
}

template <int MAP_SIZE>
typename GameCore_t<MAP_SIZE>::Vec_t GameCore_t<MAP_SIZE>::get_bird_velocity() const {
	return Vec_t{ _x_velocity, _y_velocity };
}

template <int MAP_SIZE>
int GameCore_t<MAP_SIZE>::get_current_ceil() const {
	return _curr_cell;
}

template <int MAP_SIZE>
typename GameCore_t<MAP_SIZE>::State_t GameCore_t<MAP_SIZE>::get_state() const {
	return _state;
}

template <int MAP_SIZE>
const typename GameCore_t<MAP_SIZE>::MapElement_t* GameCore_t<MAP_SIZE>::get_map() {
	return _map;
}

#include "game_core.h"

GameCore_t::GameCore_t(
	size_t map_size, float x_velocity, float y_accel, float y_flap_velocity, float tube_dist, float tube_width, 
	float hole_size, float start_pos, float bird_heiht, float bird_width) :

	_x_pos(start_pos), _y_pos(0.5), _x_velocity(x_velocity), _y_velocity(0), 
	_y_accel(y_accel), _y_flap_velocity(y_flap_velocity), _tube_dist(tube_dist), _tube_width(tube_width),
	_bird_height(bird_heiht), _bird_width(bird_width), _hole_size(hole_size), 
	_start_pos(start_pos), _state(PLAYING), _map(map_size), _freeze_x_pos(false)
{
	for (size_t i = 0; i < map_size; i++) {
		_map.push_back(gen_map_element(i * (tube_width + tube_dist)));
	}
	_curr_cell = _map.begin();
}

bool GameCore_t::MapElement_t::intersect(Vec_t point, Vec_t rect_size) const {
	return point.x + rect_size.x / 2 >= x_pos - width / 2 && (
		point.y + rect_size.y / 2 >= y_pos + hole_size / 2 ||
		point.y - rect_size.y / 2 <= y_pos - hole_size / 2);
}

void GameCore_t::step(float dtime, bool flap) {
	if (_state == DEAD) {
		if (_y_pos >= -1) {
			_y_velocity += _y_accel * dtime;
			_y_pos += _y_velocity * dtime;
		}
		return;
	}

	if (_curr_cell->x_pos + _tube_width / 2 <= _x_pos - _bird_width / 2) {
		if (_x_pos - _map.front().x_pos > 0.5f) {
  			_map.push_back_override(gen_map_element(_map.back().x_pos + _tube_width + _tube_dist));
		}
		++_curr_cell;
	}

	if (_curr_cell->intersect(Vec_t {_x_pos, _y_pos }, Vec_t { _bird_width, _bird_height }) || _y_pos <= -1 || _y_pos >= 1) {
		_state = DEAD;
	} else {
		_y_velocity = _y_flap_velocity * flap + (_y_velocity + _y_accel * dtime) * !flap;
		_x_pos += _x_velocity * dtime * !_freeze_x_pos;
		_y_pos += _y_velocity * dtime;
	}
}

void GameCore_t::reset() {
	_x_pos = _start_pos;
	_y_pos = 0.5;
	_y_velocity = 0;
	_state = PLAYING;

	_map.clear();

	float x_pos = 0;
	for (size_t i = 0; i < _map.size(); i++) {
		_map.push_back_override(gen_map_element(x_pos += _tube_width + _tube_dist));
	}
	_curr_cell = _map.begin();
}

GameCore_t::Vec_t GameCore_t::get_bird_pos() const {
	return Vec_t{ _x_pos, _y_pos };
}

GameCore_t::Vec_t GameCore_t::get_bird_velocity() const {
	return Vec_t{ _x_velocity, _y_velocity };
}

int GameCore_t::get_current_ceil() const {
	return _curr_cell;
}

GameCore_t::State_t GameCore_t::get_state() const {
	return _state;
}

void GameCore_t::freeze_x_pos(bool freeze) {
	_freeze_x_pos = freeze;
}

const RingBuffer_t<GameCore_t::MapElement_t>& GameCore_t::get_map() const {
	return _map;
}

GameCore_t::MapElement_t GameCore_t::gen_map_element(float x_pos) const {
	MapElement_t map_element;
	map_element.x_pos = x_pos;
	map_element.y_pos = -1.f + _hole_size / 2.f + 0.1f + (rand() % 100) / 100.f * (2.f - _hole_size - 0.2f);
	map_element.hole_size = _hole_size;
	map_element.width = _tube_width;

	return map_element;
}

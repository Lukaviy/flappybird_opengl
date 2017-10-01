#include "game_scene.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics.hpp>

const float tube_width = 0.4f;

GameScene_t::GameScene_t(GameCore_t& game_core) :
	_game_core(game_core), _flap(false), _bird_shape(sf::Vector2<float>(0.1f, 0.1f)),
	_up_tube_shape(sf::Vector2<float>(tube_width, 2.f)), _bottom_tube_shape(sf::Vector2<float>(tube_width, 2.f)),
	_game_view(sf::FloatRect(-1.f, 1.f, 2.f, -2.f)), _gui_view(sf::FloatRect(0.f, 0.f, 500.f, 500.f)),
	_state(WAIT), _elapsed_time(0)
{
	_bird_shape.setFillColor(sf::Color::Red);
	_up_tube_shape.setFillColor(sf::Color::Green);
	_bottom_tube_shape.setFillColor(sf::Color::Green);
	_bird_shape.move(-0.05f, -0.05f);
	_up_tube_shape.move(-tube_width / 2.f, 0.f);
	_bottom_tube_shape.move(-tube_width / 2.f, -2.f);

	_game_core.freeze_x_pos(true);
}

void GameScene_t::step(float dt) {
	_game_core.step(dt, _flap || _state == WAIT && _game_core.get_bird_pos().y < 0);
	_flap = false;
	if (_game_core.get_state() == GameCore_t::DEAD) {
		_state = DEAD;
	}
	_elapsed_time += dt * (_state == PLAYING);
}

void GameScene_t::send_event(sf::Event event) {
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		_flap = true;
	}
}

void GameScene_t::reset() {
	_game_core.reset();
	_state = PLAYING;
}

void GameScene_t::set_font(sf::Font font) {
	_font = font;
}

void GameScene_t::start() {
	_game_core.freeze_x_pos(false);
	_state = PLAYING;
}

GameScene_t::GameSceneState_t GameScene_t::get_state() const {
	return _state;
}

void GameScene_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	auto bird_pos = _game_core.get_bird_pos();

	auto bv = _game_core.get_bird_velocity();

	float velocity = sqrt(bv.x * bv.x + bv.y * bv.y);
	float cos_b = bv.y / velocity;
	float sin_b = bv.x / velocity;

	sf::Transform t3(
		cos_b, sin_b, 0,
		-sin_b, cos_b, bird_pos.y,
		0, 0, 1
	);

	auto map = _game_core.get_map();
	int curr_ceil = _game_core.get_current_ceil();

	target.setView(_game_view);

	for (int i = 0; i < map.size(); i++) {
		auto tube = map[(curr_ceil + i) % map.size()];

		float hole_size = tube.hole_size;
		float hole_pos = tube.y_pos;
		float x_pos = tube.x_pos - bird_pos.x;

		sf::Transform up_tube_transform;
		sf::Transform bottom_tube_transform;
		up_tube_transform.translate(x_pos, hole_pos + hole_size / 2.f);
		bottom_tube_transform.translate(x_pos, hole_pos - hole_size / 2.f);

		target.draw(_up_tube_shape, up_tube_transform);
		target.draw(_bottom_tube_shape, bottom_tube_transform);
	}

	target.draw(_bird_shape, t3);

	if (_state != WAIT) {
		sf::Text text(std::to_string(_game_core.get_current_ceil()), _font, 50.f);
		text.setFillColor(sf::Color(255, 255, 255, std::min(1.f, _elapsed_time) * 255));
		text.setOutlineColor(sf::Color::Black);
		text.move(240.f, 20.f);

		target.setView(_gui_view);
		target.draw(text);
	}

}

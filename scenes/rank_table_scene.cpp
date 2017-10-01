#include "rank_table_scene.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <corecrt_math_defines.h>

RankTableScene_t::RankTableScene_t(RankTable_t& rank_table) : 
	_rank_table(rank_table), _score(0), _elapsed_time(0), _last_type_time(0), _status(START),
	_view(sf::Rect<float>(0.f, 0.f, 500.f, 500.f)), _save_score_time(0) {}

void RankTableScene_t::step(float dt) {
	_elapsed_time += dt;

	if (_status != SCORE_SAVED && _elapsed_time > 2.0f) {
		_status = TYPING_NAME;
	}
}

void RankTableScene_t::send_event(sf::Event event) {
	if (_status == TYPING_NAME && event.type == sf::Event::TextEntered && event.text.unicode < 128 && event.text.unicode >= 48) {
		_player_name += static_cast<char>(event.text.unicode);
		_last_type_time = _elapsed_time;
	}
	if (_status == TYPING_NAME && event.type == sf::Event::KeyPressed && _elapsed_time > 2.f) {
		if (event.key.code == sf::Keyboard::BackSpace && _player_name.size() > 0) {
			_player_name.pop_back();
			_last_type_time = _elapsed_time;
		}
		if (event.key.code == sf::Keyboard::Return) {
			_rank_table.save_score(_player_name.c_str(), _score);
			_status = SCORE_SAVED;
			_save_score_time = _elapsed_time;
		}
	}
}

void RankTableScene_t::reset() {
	_elapsed_time = 0;
	_last_type_time = 0;
	_status = START;
}

void RankTableScene_t::set_size(sf::Vector2f size) {
	_size = size;
}

void RankTableScene_t::set_font(sf::Font font) {
	_font = font;
}

void RankTableScene_t::set_score(unsigned int score) {
	_score = score;
}

RankTableScene_t::RankTableSceneStatus_t RankTableScene_t::get_state() const {
	return _status;
}

void RankTableScene_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	target.setView(_view);

	sf::RectangleShape _background_rect(_size);
	_background_rect.setFillColor(sf::Color(50, 50, 50, std::min(0.5f, _elapsed_time) * 2.f * 150));

	target.draw(_background_rect, states);

	auto enter_your_name_text_color = sf::Color(255, 255, 255, std::min(0.5f, _elapsed_time) * 2.f * 255);

	sf::Text enter_your_name_text("Enter your name: ", _font, 30.f);
	enter_your_name_text.move(30.f, 20.f);
	enter_your_name_text.setFillColor(enter_your_name_text_color);
	target.draw(enter_your_name_text, states);

	sf::Text curr_player_name_text(
		_player_name + (_status == TYPING_NAME && int(floor(_elapsed_time * 2.f)) & 1 && _elapsed_time - _last_type_time > 0.5f ? "|" : ""),
		_font, 
		30.f
	);
	curr_player_name_text.setFillColor(enter_your_name_text_color);
	curr_player_name_text.move(230.f, 20.f);
	target.draw(curr_player_name_text, states);

	auto score_text_color = sf::Color(255, 255, 255, std::min(0.5f, std::max(0.f, _elapsed_time - 0.5f)) * 2.f * 255); 

	for (int i = 0; i < std::min(_rank_table.size(), size_t(10)); i++) {
		sf::Text player_name_text(_rank_table[i].player_name, _font, 20.f);
		player_name_text.setFillColor(score_text_color);
		player_name_text.move(120.f, 70 + 20.f * i);

		target.draw(player_name_text, states);
		player_name_text.setString(std::to_string(_rank_table[i].score));
		player_name_text.move(100.f + std::min(0.5f, std::max(0.f, _elapsed_time - 0.5f)) * 2.f * 30.f, 0);
		target.draw(player_name_text, states);
	}

	if (_status == SCORE_SAVED && _elapsed_time > 1.f) {
		sf::Text press_space_text("Press space to restart", _font, 20.f);
		press_space_text.setFillColor(sf::Color(255, 255, 255, (sin((_elapsed_time - _save_score_time) * 4.f - M_PI_2) + 1) / 2.f * 255));
		press_space_text.move(40.f, 460.f);
		target.draw(press_space_text);
	}
}

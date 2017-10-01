#include "rank_table_scene.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

RankTableScene_t::RankTableScene_t(RankTable_t& rank_table) : _rank_table(rank_table), _score(0), _elapsed_time(0), _last_type_time(0) {}

void RankTableScene_t::step(float dt) {
	_elapsed_time += dt;
}

void RankTableScene_t::send_event(sf::Event event) {
	if (event.type == sf::Event::TextEntered && event.text.unicode < 128 && event.text.unicode >= 48) {
		_player_name += static_cast<char>(event.text.unicode);
		_last_type_time = _elapsed_time;
	}
	if (event.type == sf::Event::KeyPressed && _elapsed_time > 2.f) {
		if (event.key.code == sf::Keyboard::BackSpace && _player_name.size() > 0) {
			_player_name.pop_back();
			_last_type_time = _elapsed_time;
		}
		if (event.key.code == sf::Keyboard::Return) {
			_rank_table.save_score(_player_name.c_str(), _score);
		}
	}
}

void RankTableScene_t::reset() {
	_elapsed_time = 0;
	_last_type_time = 0;
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

void RankTableScene_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	sf::RectangleShape _background_rect(_size);
	_background_rect.setFillColor(sf::Color(50, 50, 50, std::min(0.5f, _elapsed_time) * 2.f * 150));

	target.draw(_background_rect, states);

	auto enter_your_name_text_color = sf::Color(255, 255, 255, std::min(0.5f, _elapsed_time) * 2.f * 255);

	sf::Text enter_your_name_text("Enter your name: ", _font, 30.f);
	enter_your_name_text.move(30.f, 20.f);
	enter_your_name_text.setFillColor(enter_your_name_text_color);
	target.draw(enter_your_name_text, states);

	sf::Text curr_player_name_text(_player_name + (int(floor(_elapsed_time)) & 1 && _elapsed_time - _last_type_time > 1 ? "|" : ""), _font, 30.f);
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
		player_name_text.move(100.f, 0);
		target.draw(player_name_text, states);
	}
}

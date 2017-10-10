#include "rank_table_scene.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <corecrt_math_defines.h>
#include "../animation/linear_animation.h"
#include "../animation/timer_animation.h"
#include "../animation/sin_animation.h"
#include "../animation/starter_animation.h"

RankTableScene_t::RankTableScene_t(RankTable_t& rank_table, sf::Font font) : 
	_rank_table(rank_table), _score(0), _last_type_time(0), _status(START),
	_view(sf::Rect<float>(0.f, 0.f, 500.f, 500.f)), _save_score_time(0), _place(0), _font(font)
{
	_enter_your_name_text = sf::Text("Enter your name: ", _font, 30);
	_curr_player_name_text = sf::Text("", _font, 30);
	_press_space_text = sf::Text("Press space to restart", _font, 20);
	_best_score_text = sf::Text("Best score: ", _font, 30);


	_enter_your_name_text.setPosition(30.f, 20.f);
	_press_space_text.setPosition(40.f, 460.f);
	_background_rect.setSize(_size);

	_appearence_background_animation = _animator.make<LinearAnimation_t>(0, 150, 0.5f)
		.on_stop([this]() { _status = TYPING_NAME; });

	_appearence_text_color_animation = _animator.make<LinearAnimation_t>(0, 255, 0.7f)
		.start_after(_animator.make<TimerAnimation_t>(0.4f).start_with(_appearence_background_animation));

	_score_appearence_animation = _animator.make<LinearAnimation_t>(0.f, 30.f, 0.5f)
		.start_after(_animator.make<TimerAnimation_t>(0.4f).start_with(_appearence_background_animation));

	_press_space_animation = _animator.make<SinAnimation_t>(0, 255, 5, -M_PI_2);
	_best_score_appearence_animation = _animator.make<LinearAnimation_t>(0.f, 25.f, 0.3f).
		start_with(_press_space_animation);

	_enter_your_name_text_color_dissapearing = _animator.make<LinearAnimation_t>(0, -255, 0.5f);

	_status = START;
	_appearence_background_animation.start();
}

void RankTableScene_t::reset() {
	_status = START;
	_animator.reset();

	_appearence_background_animation.start();
}

void RankTableScene_t::step(float dt) {
	_animator.step(dt);

	_background_rect.setFillColor(sf::Color(50, 50, 50, _appearence_background_animation.val()));
 	_enter_your_name_text.setFillColor(sf::Color(255, 255, 255, 
		std::max(_appearence_text_color_animation.val() + _enter_your_name_text_color_dissapearing.val(), 0.f)));
	_enter_your_name_text.setPosition(30.f + _enter_your_name_dissapearing.val_or(0), 20.f);
	_curr_player_name_text.setString(
		_player_name + (_status == TYPING_NAME && 
			int(floor(_animator.elapsed_time() * 2.f)) & 1 && _animator.elapsed_time() - _last_type_time > 0.5f ? "|" : "")
	);
	_curr_player_name_text.setFillColor(sf::Color(255, 255, 255, _appearence_text_color_animation.val()));
	_curr_player_name_text.setPosition(230.f + _enter_your_name_dissapearing.val_or(0), 20.f);
	_press_space_text.setFillColor(sf::Color(255, 255, 255, _press_space_animation.val()));
	_score_text_color = sf::Color(255, 255, 255, _appearence_text_color_animation.val());

	if (_status == SCORE_SAVED) {
		_best_score_text.setString(std::string("Best score: ") + std::to_string(_rank_table[_player_name].score));
		_best_score_text.setFillColor(sf::Color(255, 255, 255, _best_score_text_color_animation.val()));
		_best_score_text.setPosition(_best_score_appearence_animation.val(), 20.f);
	}
}

void RankTableScene_t::send_event(sf::Event event) {
	if (_status == TYPING_NAME && event.type == sf::Event::TextEntered && (
		event.text.unicode >= 'a' && event.text.unicode <= 'z' ||
		event.text.unicode >= 'A' && event.text.unicode <= 'Z' ||
		event.text.unicode >= '0' && event.text.unicode <= '9') &&
		_player_name.size() < Rank_t::MAX_PLAYER_NAME_LEN
	) {
		_player_name += static_cast<char>(event.text.unicode);
		_last_type_time = _animator.elapsed_time();
	}
	if (_status == TYPING_NAME && event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::BackSpace && _player_name.size() > 0) {
			_player_name.pop_back();
			_last_type_time = _animator.elapsed_time();
		}
		if (event.key.code == sf::Keyboard::Return && !_player_name.empty()) {
			_rank_table.save_score(_player_name.c_str(), _score);
			_status = SCORE_SAVED;
			_press_space_animation.start();

			_enter_your_name_dissapearing = _animator.make<LinearAnimation_t>(0, -200.f, 0.5f);
			_enter_your_name_text_color_dissapearing.start_with(_enter_your_name_dissapearing);

			_best_score_appearence_animation = _animator.make<LinearAnimation_t>(
				_curr_player_name_text.getGlobalBounds().left + _curr_player_name_text.getGlobalBounds().width + 10.f, 
				50.f + _curr_player_name_text.getLocalBounds().width, 0.5f
				).start_with(_enter_your_name_dissapearing);

			_enter_your_name_dissapearing.start();
		}
	} else if (_status == SCORE_SAVED && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		_status = RESTART_REQUESTED;
	}
}

void RankTableScene_t::set_size(sf::Vector2f size) {
	_size = size;
	_background_rect.setSize(_size);
}

void RankTableScene_t::set_font(const sf::Font& font) {
	_font = font;
	_press_space_text.setFont(_font);
	_curr_player_name_text.setFont(_font);
	_enter_your_name_text.setFont(_font);
}

void RankTableScene_t::set_score(unsigned int score) {
	_score = score;
	_place = _rank_table.find_place(score);
}

RankTableScene_t::RankTableSceneStatus_t RankTableScene_t::get_state() const {
	return _status;
}

void RankTableScene_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	target.setView(_view);

	target.draw(_background_rect, states);
	target.draw(_enter_your_name_text, states);
	target.draw(_curr_player_name_text, states);
	
	if (_status == SCORE_SAVED && _rank_table.exists(_player_name)) {
		target.draw(_best_score_text, states);
	}
	static unsigned const int list_size = 10;
	unsigned int start = 0;
	unsigned int end = 0;
	if (_place <= list_size / 2) {
		start = 0;
		end = std::min(list_size, _rank_table.size());
	} else if (_place >= _rank_table.size() - list_size / 2) {
		if (_rank_table.size() <= list_size) {
			start = 0;
			end = _rank_table.size();
		} else {
			start = _rank_table.size() - list_size - 1;
			end = _rank_table.size() - 1;
		}
	} else {
		start = _place - list_size / 2;
		end = _place + (list_size - list_size / 2);
	}

 	for (unsigned int i = start; i < end; i++) {
		sf::Text text(std::to_string(i + 1) + '.', _font, 20.f);
		float width = text.getLocalBounds().width;
		text.setPosition(110.f - width, + 70 + 20.f * (i - start));
		text.setFillColor(_score_text_color);
		target.draw(text, states);

		text.setString(_rank_table[i].player_name);
		text.setFillColor(_score_text_color);
		text.move(width + 15.f, 0);
		target.draw(text, states);

		text.setString(std::to_string(_rank_table[i].score));
		text.move(100.f + _score_appearence_animation.val(), 0);
		target.draw(text, states);
	}

	if (_status == SCORE_SAVED) {
		target.draw(_press_space_text);
	}
}

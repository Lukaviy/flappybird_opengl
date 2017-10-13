#include "rank_table_scene.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <corecrt_math_defines.h>
#include "../animation/linear_animation.h"
#include "../animation/timer_animation.h"
#include "../animation/sin_animation.h"

RankTableScene_t::RankTableScene_t(RankTable_t& rank_table, sf::Font font) : 
	_rank_table(rank_table), _score(0), _last_type_time(0), _state(START),
	_view(sf::Rect<float>(0.f, 0.f, 500, 500)), _save_score_time(0), _place(0), _font(font)
{
	_enter_your_name_text = sf::Text("Enter your name: ", _font, 30);
	_curr_player_name_text = sf::Text("", _font, 30);
	_press_space_text = sf::Text("Press space to restart", _font, 20);
	_best_score_text = sf::Text("Best score: ", _font, 30);

	_enter_your_name_text.setPosition(30.f, 20.f);
	_press_space_text.setPosition(40.f, 460.f);
	_background_rect.setSize(_size);

	_state = START;

	_canvas = new GuiElement_t();
	_canvas->size(500, 500);

	_main_panel = new GuiAlignPanel_t;
	_main_panel
		->direction(GuiAlignPanel_t::BOTTOM_TO_TOP)
		->visible(false)
		->size(400, 400)
		->align(GuiElement_t::BOTTOM)
		->position(GuiElement_t::Position_t::absolute(0, 0))
		->parent(_canvas);

	_press_space_label = new GuiLabel_t;
	_press_space_label
		->text("Press space to restart")
		->font(_font)
		->char_size(20)
		->visible(false)
		->align(GuiElement_t::LEFT_BOTTOM)
		->padding(GuiElement_t::Padding_t(0, 10, 0, 10))
		->parent(_main_panel);

	_results_table_panel = new GuiAlignPanel_t();
	_results_table_panel
		->background_color(sf::Color(50, 50, 50, 150))
		->size(400, 300)
		->margin(25)
		->align(GuiElement_t::TOP)
		->parent(_main_panel);

	_top_text_panel = new GuiAlignPanel_t();
	_top_text_panel
		->direction(GuiAlignPanel_t::TOP_TO_BOTTOM)
		->visible(false)
		->padding(GuiElement_t::Padding_t(5, 10, 0, 0))
		->size(0, 30)
		->parent(_results_table_panel);

	_enter_your_name_panel = new GuiAlignPanel_t();
	_enter_your_name_panel
		->direction(GuiAlignPanel_t::LEFT_TO_RIGHT)
		->background_color(sf::Color(50, 0, 0, 50))
		->visible(false)
		->size(300, 30)
		->parent(_top_text_panel);

	_enter_your_name_label = new GuiLabel_t();
	_enter_your_name_label
		->text("Enter your name: ")
		->font(_font)
		->char_size(30)
		->visible(false)
		->parent(_enter_your_name_panel);

	_player_name_label = new GuiLabel_t();
	_player_name_label
		->text("")
		->font(_font)
		->char_size(30)
		->visible(false)
		->parent(_enter_your_name_panel);

	_best_score_panel = new GuiAlignPanel_t();
	_best_score_panel
		->direction(GuiAlignPanel_t::LEFT_TO_RIGHT)
		->visible(false)
		->parent(_top_text_panel);

	_best_score_text_label = new GuiLabel_t();
	_best_score_text_label
		->text("BEST SCORE: ")
		->font(_font)
		->char_size(30)
		->visible(false)
		->parent(_best_score_panel);

	_best_score_label = new GuiLabel_t();
	_best_score_label
		->font(_font)
		->char_size(30)
		->visible(false)
		->parent(_best_score_panel);

	_rank_table_panel = new GuiAlignPanel_t();
	_rank_table_panel
		->visible(false)
		->enabled(false)
		->align(GuiElement_t::TOP)
		->size(200, 300)
		->parent(_results_table_panel);

	_background_appearance_animation = _animator.make<LinearAnimation_t>(0, 150, 0.5f)
		.on_start([this]() { _results_table_panel->visible(true); })
		.on_reset([this]() { _results_table_panel->visible(false); })
		.on_stop([this]() { _state = TYPING_NAME; })
		.on_step([this](float val)
	{
		auto color = _results_table_panel->background_color();
		_results_table_panel->background_color(sf::Color(color.r, color.g, color.b, val));
	});

	_enter_name_appearance_animation = _animator.make<LinearAnimation_t>(0, 255, 0.7f)
		.start_after(_background_appearance_animation)
		.on_start([this]()
	{
		_enter_your_name_label->visible(true);
		_player_name_label->visible(true);
		_enter_your_name_label->color(sf::Color::Transparent);
		_player_name_label->color(sf::Color::Transparent);
	})
		.on_reset([this]()
	{
		_enter_your_name_label->visible(false);
		_player_name_label->visible(false);
	})
		.on_step([this](float val)
	{
		_enter_your_name_label->color(sf::Color(255, 255, 255, val));
		_player_name_label->color(sf::Color(255, 255, 255, val));
	});

	_rank_table_appearence_animation = _animator.make<LinearAnimation_t>(0, 255, 0.5f)
		.start_after(_enter_name_appearance_animation)
		.on_start([this]()
	{
		_rank_table_panel->enabled(true);

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
			auto rank_row = (new GuiElement_t())
				->parent(_rank_table_panel);

			auto place_panel = (new GuiElement_t())
				->parent(rank_row);

			auto score_panel = (new GuiElement_t)
				->align(GuiElement_t::TOP_RIGHT)
				->parent(rank_row);

			auto place_label = new GuiLabel_t();
			place_label
				->text(std::to_string(i + 1) + '.')
				->font(_font)
				->char_size(20)
				->color(sf::Color::Transparent)
				->align(GuiElement_t::TOP_RIGHT)
				->parent(place_panel);

			auto player_name_label = new GuiLabel_t();
			player_name_label
				->text(_rank_table[i].player_name)
				->font(_font)
				->char_size(20)
				->color(sf::Color::Transparent)
				->padding(GuiElement_t::Padding_t(5, 0, 0, 0))
				->parent(rank_row);
				
			auto player_score_label = new GuiLabel_t();
			player_score_label
				->text(std::to_string(_rank_table[i].score))
				->font(_font)
				->char_size(20)
				->color(sf::Color::Transparent)
				->parent(score_panel);

			_rank_labels.push_back(place_label);
			_rank_labels.push_back(player_name_label);
			_rank_labels.push_back(player_score_label);
			_rank_score_labels.push_back(score_panel);

			rank_row
				->size(_rank_table_panel->size().x, place_label->size().y)
				->padding(3);
		}
	})
		.on_reset([this]() 
	{
		_rank_table_panel->visible(false);
		_rank_table_panel->clear();
		_rank_labels.clear();
		_rank_score_labels.clear();
	})
		.on_step([this](float val)
	{
		for (auto label : _rank_labels) {
			label->color(sf::Color(255, 255, 255, val));
		}
	});

	_players_scores_appearence_animation = _animator.make<LinearAnimation_t>(-30, 0, 0.5f)
		.start_with(_rank_table_appearence_animation)
		.on_step([this](float val)
	{
		for (auto element : _rank_score_labels) {
			element->position(GuiElement_t::Position_t::absolute(val, 0));
		}
	});

	_press_space_animation = _animator.make<SinAnimation_t>(0, 255, 5.f)
		.on_start([this]() { _press_space_label->visible(true); })
		.on_reset([this]() { _press_space_label->visible(false); })
		.on_step([this](float val)
	{
		_press_space_label->color(sf::Color(255, 255, 255, val));
	});

	_rank_table_down_animation = _animator.make<LinearAnimation_t>(30, 60, 0.5f)
		.start_with(_press_space_animation)
		.on_step([this](float val)
	{
		_top_text_panel->size(0, val);
	})
		.on_reset([this]()
	{
		_top_text_panel->size(0, 30);
	});

	_best_score_appearence_animation = _animator.make<LinearAnimation_t>(0, 255, 0.5f)
		.start_after(_rank_table_down_animation)
		.on_start([this]()
	{
		_best_score_text_label->visible(true);
		_best_score_label->visible(true);
		_best_score_text_label->color(sf::Color::Transparent);
		_best_score_label->color(sf::Color::Transparent);
	})
		.on_step([this](float val)
	{
		_best_score_text_label->color(sf::Color(255, 255, 255, val));
		_best_score_label->color(sf::Color(255, 255, 255, val));
	})
		.on_reset([this]()
	{
		_best_score_text_label->color(sf::Color::Transparent);
		_best_score_label->color(sf::Color::Transparent);
	});

	_background_appearance_animation.start();
}

void RankTableScene_t::reset() {
	_state = START;
	_animator.reset();

	_background_appearance_animation.start();
}

void RankTableScene_t::step(float dt) {
	_animator.step(dt);

	_player_name_label->text(
		_player_name + (_state == TYPING_NAME &&
			int(floor(_animator.elapsed_time() * 2.f)) & 1 && _animator.elapsed_time() - _last_type_time > 0.5f ? "|" : "")
	);
}

void RankTableScene_t::send_event(sf::Event event) {
	if (_state == TYPING_NAME && event.type == sf::Event::TextEntered && (
		event.text.unicode >= 'a' && event.text.unicode <= 'z' ||
		event.text.unicode >= 'A' && event.text.unicode <= 'Z' ||
		event.text.unicode >= '0' && event.text.unicode <= '9') &&
		_player_name.size() < Rank_t::MAX_PLAYER_NAME_LEN
	) {
		_player_name += static_cast<char>(event.text.unicode);
		_last_type_time = _animator.elapsed_time();
	}
	if (_state == TYPING_NAME && event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::BackSpace && _player_name.size() > 0) {
			_player_name.pop_back();
			_last_type_time = _animator.elapsed_time();
		}
		if (event.key.code == sf::Keyboard::Return && !_player_name.empty()) {
			_state = SCORE_SAVED;

			_rank_table.save_score(_player_name.c_str(), _score);
			_best_score_label->text(std::to_string(_rank_table[_player_name].score));

			_press_space_animation.start();
		}
	} else if (_state == SCORE_SAVED && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		_state = RESTART_REQUESTED;
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
	return _state;
}

void RankTableScene_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	target.setView(_view);

	target.draw(*_canvas);
}

#pragma once
#include "scene.h"
#include "../rank_table/rank_table.h"
#include "../animation/animator.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>

class RankTableScene_t : public Scene_t {
public:
	~RankTableScene_t() override = default;
	RankTableScene_t(RankTable_t& rank_table, sf::Font font);
	void step(float dt) override;
	void send_event(sf::Event event) override;
	void reset() override;
	void set_size(sf::Vector2f size);
	void set_font(const sf::Font& font);
	void set_score(unsigned int score);

	enum RankTableSceneStatus_t {
		START,
		TYPING_NAME,
		SCORE_SAVED,
		RESTART_REQUESTED,
	};

	RankTableSceneStatus_t get_state() const;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Animator_t _animator;
	
	sf::Vector2f _size;
	sf::Vector2f _label_position;

	RankTable_t& _rank_table;

	sf::RectangleShape _background_rect;
	sf::Text _enter_your_name_text;
	sf::Text _curr_player_name_text;
	sf::Text _press_space_text;
	sf::Text _best_score_text;
	sf::Color _score_text_color;

	sf::View _view;

	sf::Font _font;

	AnimPointer_t _appearence_background_animation;
	AnimPointer_t _appearence_text_color_animation;
	AnimPointer_t _score_appearence_animation;
	AnimPointer_t _press_space_animation;
	AnimPointer_t _best_score_appearence_animation;
	AnimPointer_t _best_score_text_color_animation;
	AnimPointer_t _enter_your_name_dissapearing;
	AnimPointer_t _enter_your_name_text_color_dissapearing;

	std::string _player_name;
	unsigned int _score;
	unsigned int _place;
	float _last_type_time;
	float _save_score_time;
	RankTableSceneStatus_t _state;
};

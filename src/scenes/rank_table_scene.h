#pragma once
#include "scene.h"
#include "../rank_table/rank_table.h"
#include "../animation/animator.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../gui/gui_element.h"
#include "../gui/gui_panel.h"
#include "../gui/gui_align_panel.h"
#include "../gui/gui_label.h"

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

	GuiElement_t* _canvas;
	GuiAlignPanel_t* _main_panel;
	GuiLabel_t* _press_space_label;
	GuiAlignPanel_t* _results_table_panel;
	GuiAlignPanel_t* _enter_your_name_panel;
	GuiLabel_t* _enter_your_name_label;
	GuiLabel_t* _player_name_label;
	GuiAlignPanel_t* _top_text_panel;
	GuiAlignPanel_t* _rank_table_panel;
	GuiAlignPanel_t* _best_score_panel;
	GuiLabel_t* _best_score_text_label;
	GuiLabel_t* _best_score_label;

	sf::View _view;

	sf::Font _font;

	AnimPointer_t _background_appearance_animation;
	AnimPointer_t _enter_name_appearance_animation;
	AnimPointer_t _rank_table_appearence_animation;
	AnimPointer_t _players_scores_appearence_animation;
	AnimPointer_t _press_space_animation;
	AnimPointer_t _rank_table_down_animation;
	AnimPointer_t _best_score_appearence_animation;
	std::vector<GuiLabel_t*> _rank_labels;
	std::vector<GuiElement_t*> _rank_score_labels;

	std::string _player_name;
	unsigned int _score;
	unsigned int _place;
	float _last_type_time;
	float _save_score_time;
	RankTableSceneStatus_t _state;
};

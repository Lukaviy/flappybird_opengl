#pragma once
#include "../rank_table/rank_table.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include "scene.h"
#include <SFML/Graphics/View.hpp>

class RankTableScene_t : public Scene_t {
public:
	~RankTableScene_t() override = default;
	RankTableScene_t(RankTable_t& rank_table);
	void step(float dt) override;
	void send_event(sf::Event event) override;
	void reset() override;
	void set_size(sf::Vector2f size);
	void set_font(sf::Font font);
	void set_score(unsigned int score);

	enum RankTableSceneStatus_t {
		START,
		TYPING_NAME,
		SCORE_SAVED,
	};

	RankTableSceneStatus_t get_state() const;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	sf::Vector2f _size;
	sf::Vector2f _label_position;

	RankTable_t& _rank_table;

	sf::RectangleShape _background_rect;
	sf::Font _font;
	sf::View _view;

	std::string _player_name;
	unsigned int _score;
	float _elapsed_time;
	float _last_type_time;
	float _save_score_time;
	RankTableSceneStatus_t _status;
};

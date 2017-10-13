#pragma once
#include "scene.h"
#include "../engine/game_core.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>

class GameScene_t : public Scene_t {
public:
	GameScene_t(GameCore_t& game_core);
	void step(float dt) override;
	void send_event(sf::Event event) override;
	void reset() override;
	void set_font(sf::Font font);
	void start();

	enum GameSceneState_t {
		WAIT,
		PLAYING,
		DEAD
	};

	GameSceneState_t get_state() const;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	GameCore_t& _game_core;
	bool _flap;
	GameSceneState_t _state;
	float _elapsed_time;

	sf::RectangleShape _bird_shape;
	sf::RectangleShape _up_tube_shape;
	sf::RectangleShape _bottom_tube_shape;
	sf::Font _font;
	sf::View _gui_view;
	sf::View _game_view;
};

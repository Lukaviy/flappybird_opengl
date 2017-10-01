#pragma once
#include "scene.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>

class MainTitleScene_t : public Scene_t {
public:
	MainTitleScene_t();
	void step(float dt) override;
	void reset() override;
	void set_font(sf::Font font);
	void send_event(sf::Event event) override;
	~MainTitleScene_t() override = default;

	enum MainTitleSceneStatus_t {
		SHOWING,
		WAIT_FOR_START,
		STARTED,
	};

	MainTitleSceneStatus_t get_state() const;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Font _font;
	sf::View _view;

	float _elapsed_time;
	MainTitleSceneStatus_t _status;
};

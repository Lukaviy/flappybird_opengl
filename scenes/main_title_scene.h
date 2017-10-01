#pragma once
#include "scene.h"
#include "../utils/animation.h"
#include "../utils/animator.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>

class MainTitleScene_t : public Scene_t {
public:
	MainTitleScene_t(const sf::Font& font);
	void reset() override;
	void set_font(const sf::Font& font);
	void send_event(sf::Event event) override;
	void step(float dt) override;
	~MainTitleScene_t() override = default;

	enum MainTitleSceneStatus_t {
		SHOWING,
		WAIT_FOR_START,
		STARTED,
	};

	MainTitleSceneStatus_t get_state() const;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	Animator_t _animator;

	sf::View _view;

	MainTitleSceneStatus_t _status;

	AnimPointer_t _title_appearence_animation;
	AnimPointer_t _press_space_animation;
	sf::Text _main_title_text;
	sf::Text _press_space_text;
};

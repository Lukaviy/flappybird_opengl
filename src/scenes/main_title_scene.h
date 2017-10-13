#pragma once
#include "scene.h"
#include "../animation/animator.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../gui/gui_element.h"
#include "../gui/gui_label.h"
#include "../gui/gui_align_panel.h"

class MainTitleScene_t : public Scene_t {
public:
	MainTitleScene_t(const sf::Font& font);
	~MainTitleScene_t() override;
	void reset() override;
	void set_font(const sf::Font& font) const;
	void send_event(sf::Event event) override;
	void step(float dt) override;

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
	GuiElement_t* _canvas;
	GuiPanel_t* _panel;
	GuiLabel_t* _main_title_label;
	GuiLabel_t* _press_space_label;
};

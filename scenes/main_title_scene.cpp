#include "main_title_scene.h"
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../animation/linear_animation.h"
#include "../animation/sin_animation.h"
#include "../gui/gui_label.h"

MainTitleScene_t::MainTitleScene_t(const sf::Font& font) : 
	_view(sf::Rect<float>(0.f, 0.f, 1000.f, 1000.f)), _status(SHOWING)
{
	MainTitleScene_t::reset();

	_title_appearence_animation = _animator.make<LinearAnimation_t>(0, 255, 3);
	_press_space_animation = _animator.make<SinAnimation_t>(0, 255, 5, -M_PI_2);
	_press_space_animation.start_after(_title_appearence_animation);
	_title_appearence_animation.on_stop([this]() { _status = WAIT_FOR_START; });
	_title_appearence_animation.start();

	_canvas = new GuiElement_t();
	_canvas->size(1000.f, 1000.f);

	_main_title_label = new GuiLabel_t();
	_main_title_label
		->text("FLAPPY CUBE")
		->font(font)
		->char_size(150)
		->align(GuiElement_t::TOP)
		->position(GuiElement_t::Position_t::relative(0, .2f))
		->parent(_canvas);

	_press_space_label = new GuiLabel_t();
	_press_space_label
		->text("Press space to start")
		->font(font)
		->char_size(40)
		->align(GuiElement_t::CENTER)
		->parent(_canvas);
}

void MainTitleScene_t::reset() {
	_animator.reset();
}

void MainTitleScene_t::set_font(const sf::Font& font) const {
	_main_title_label->font(font);
	_press_space_label->font(font);
}

void MainTitleScene_t::send_event(sf::Event event) {
	if (_status == WAIT_FOR_START && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		_status = STARTED;
	}
}

MainTitleScene_t::MainTitleSceneStatus_t MainTitleScene_t::get_state() const {
	return _status;
}

void MainTitleScene_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.setView(_view);
	target.draw(*_canvas);
}

void MainTitleScene_t::step(float dt) {
	_animator.step(dt);

	_main_title_label->color(sf::Color(255, 255, 255, _title_appearence_animation.val()));
	_press_space_label->color(sf::Color(255, 255, 255, _press_space_animation.val()));
}

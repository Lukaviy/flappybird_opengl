#include "main_title_scene.h"
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../animation/linear_animation.h"
#include "../animation/sin_animation.h"

MainTitleScene_t::MainTitleScene_t(const sf::Font& font) : 
	_view(sf::Rect<float>(0.f, 0.f, 1000.f, 1000.f)), _status(SHOWING), 
	_main_title_text("FLAPPY BIRD", font, 150.f), _press_space_text("Press space to start", font, 40.f)
{
	MainTitleScene_t::reset();
}

void MainTitleScene_t::reset() {
	_title_appearence_animation = _animator.make<LinearAnimation_t>(0, 255, 3);
	_press_space_animation = _animator.make<SinAnimation_t>(0, 255, 5, -M_PI_2);
	_press_space_animation.start_after(_title_appearence_animation);
	_title_appearence_animation.on_stop([this]() { _status = WAIT_FOR_START; });
	_title_appearence_animation.start();

	_main_title_text.setPosition(200, 200);
	_press_space_text.setPosition(350, 500);
}

void MainTitleScene_t::set_font(const sf::Font& font) {
	_main_title_text.setFont(font);
	_press_space_text.setFont(font);
}

void MainTitleScene_t::send_event(sf::Event event) {
	if (_status == WAIT_FOR_START && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		_status = STARTED;
		_press_space_animation.stop();
	}
}

MainTitleScene_t::MainTitleSceneStatus_t MainTitleScene_t::get_state() const {
	return _status;
}

void MainTitleScene_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.setView(_view);
	target.draw(_main_title_text);

	if (_press_space_animation.playing()) {
		target.draw(_press_space_text);
	}
}

void MainTitleScene_t::step(float dt) {
	_animator.step(dt);

	_main_title_text.setFillColor(sf::Color(255, 255, 255, _title_appearence_animation.val()));

	if (_press_space_animation.playing()) {
		_press_space_text.setFillColor(sf::Color(255, 255, 255, _press_space_animation.val()));
	}
}

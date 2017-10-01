#include "main_title_scene.h"
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

MainTitleScene_t::MainTitleScene_t() : 
	_elapsed_time(0), _view(sf::Rect<float>(0.f, 0.f, 1000.f, 1000.f)), _status(SHOWING) {}

void MainTitleScene_t::step(float dt) {
	_elapsed_time += dt;

	if (_elapsed_time > 5.f) {
		_status = WAIT_FOR_START;
	}
}

void MainTitleScene_t::reset() {
	_elapsed_time = 0;
}

void MainTitleScene_t::set_font(sf::Font font) {
	_font = font;
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

	sf::Text main_title_text("FLAPPY BIRD", _font, 150.f);
	main_title_text.setFillColor(sf::Color(255, 255, 255, std::min(3.f, std::max(0.f, _elapsed_time - 1.f)) / 3.f * 255));
	main_title_text.move(200, 200);
	target.draw(main_title_text);

	if (_elapsed_time > 5.f) {
		sf::Text press_space_text("Press space to start", _font, 40.f);
		press_space_text.setFillColor(sf::Color(255, 255, 255, (sin((_elapsed_time - 5.f) * 4.f - M_PI_2) + 1.f) / 2.f * 255));
		press_space_text.move(350, 500);
		target.draw(press_space_text);
	}
}

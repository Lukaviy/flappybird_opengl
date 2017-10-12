#include "gui_label.h"
#include <SFML/Graphics.hpp>

void GuiLabel_t::text(sf::Text text) {
	_text = text;
	update();
}

sf::Text& GuiLabel_t::text() {
	return _text;
}

void GuiLabel_t::on_draw(sf::RenderTarget& target, sf::RenderStates states) const {
	/*sf::RectangleShape rect(_size);
	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color::Red);

	sf::CircleShape origin(10.f);
	origin.setPosition(sf::Vector2f(-10, -10) + getOrigin());
	origin.setFillColor(sf::Color::Green);

	target.draw(rect, states);
	target.draw(origin, states);
	*/
	target.draw(_text, states);
}

void GuiLabel_t::on_update() {
	auto local_bound = _text.getLocalBounds();
	_size.x = local_bound.width;
	_size.y = local_bound.height;
}

sf::Vector2f GuiLabel_t::origin_offset() const {
	auto local_bound = _text.getLocalBounds();
	return sf::Vector2f(local_bound.left, local_bound.top);
}

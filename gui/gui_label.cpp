#include "gui_label.h"
#include <SFML/Graphics.hpp>

void GuiLabel_t::text(sf::Text text) {
	_text = text;
}

sf::Text& GuiLabel_t::text() {
	return _text;
}

sf::Vector2f GuiLabel_t::size() {
	auto local_bounds = _text.getLocalBounds();
	return sf::Vector2f(local_bounds.width, local_bounds.height);
}

void GuiLabel_t::on_draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(_text, states);
}

#include "gui_label.h"
#include <SFML/Graphics.hpp>

GuiLabel_t* GuiLabel_t::text(const std::string& val) {
	_text.setString(val);
	update();
	return this;
}

std::string GuiLabel_t::text() const {
	return _text.getString();
}

GuiLabel_t* GuiLabel_t::color(sf::Color val) {
	_text.setFillColor(val);
	return this;
}

sf::Color GuiLabel_t::color() const {
	return _text.getFillColor();
}

GuiLabel_t* GuiLabel_t::font(const sf::Font& val) {
	_text.setFont(val);
	return this;
}

const sf::Font* GuiLabel_t::font() const {
	return _text.getFont();
}

GuiLabel_t* GuiLabel_t::char_size(size_t val) {
	_text.setCharacterSize(val);
	update();
	return this;
}

size_t GuiLabel_t::char_size() const {
	return _text.getCharacterSize();
}

void GuiLabel_t::on_draw(sf::RenderTarget& target, sf::RenderStates states) const {
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

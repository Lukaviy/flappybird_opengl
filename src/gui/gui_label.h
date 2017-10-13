#pragma once
#include "gui_element.h"
#include <SFML/Graphics/Text.hpp>

class GuiLabel_t : public GuiElement_t {
public:
	GuiLabel_t* text(const std::string& val);
	std::string text() const;
	GuiLabel_t* color(sf::Color val);
	sf::Color color() const;
	GuiLabel_t* font(const sf::Font& val);
	const sf::Font* font() const;
	GuiLabel_t* char_size(size_t val);
	size_t char_size() const;
protected:
	void on_draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void on_update() override;
	sf::Vector2f origin_offset() const override;
	sf::Text _text;
};

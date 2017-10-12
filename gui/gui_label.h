#pragma once
#include "gui_element.h"
#include <SFML/Graphics/Text.hpp>

class GuiLabel_t : public GuiElement_t {
public:
	void text(sf::Text text);
	sf::Text& text();
	sf::Vector2f size() override;
protected:
	void on_draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Text _text;
};

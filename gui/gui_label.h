#pragma once
#include "gui_element.h"
#include <SFML/Graphics/Text.hpp>

class GuiLabel_t : public GuiElement_t {
public:
	GuiLabel_t* text(sf::Text text);
	sf::Text& text();
protected:
	void on_draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void on_update() override;
	sf::Vector2f origin_offset() const override;
	sf::Text _text;
};

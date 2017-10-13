#pragma once

#include "gui_element.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class GuiPanel_t : public GuiElement_t {
public:
	GuiPanel_t* background_color(sf::Color color);
	sf::Color background_color() const;
protected:
	void on_draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void on_update() override;
private:
	sf::RectangleShape _rect_shape;
};

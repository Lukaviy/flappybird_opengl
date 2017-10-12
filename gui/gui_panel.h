#pragma once

#include "gui_element.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class GuiPanel_t : public GuiElement_t {
protected:
	void on_draw(sf::RenderTarget& target) const override;
	void on_update() override;
	void background_color(sf::Color color);
private:
	sf::RectangleShape _rect_shape;
};

#include "gui_panel.h"

void GuiPanel_t::on_draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(_rect_shape, states);
}

void GuiPanel_t::on_update() {
	_rect_shape.setSize(_size);
}

void GuiPanel_t::background_color(sf::Color color) {
	_rect_shape.setFillColor(color);
}

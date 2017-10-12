#include "gui_align_panel.h"

void GuiAlignPanel_t::on_update() {
	float pos = 
		_direction == BOTTOM_TO_TOP ? _size.y :
		_direction == RIGHT_TO_LEFT ? _size.x : 0;
	for (auto element : _childs) {
		switch (_direction) {
		case TOP_TO_BOTTOM: {
			element->setPosition(sf::Vector2f(0, pos));
			pos += element->bound().y;
		} break;
		case LEFT_TO_RIGHT: {
			element->setPosition(sf::Vector2f(pos, 0));
			pos += element->bound().x;
		} break;
		case BOTTOM_TO_TOP: {
			element->setPosition(sf::Vector2f(pos, 0));
			pos -= element->bound().x;
		} break;
		case RIGHT_TO_LEFT: {
			element->setPosition(sf::Vector2f(0, pos));
			pos -= element->bound().x;
		} break;
			default: ;
		}
	}
}

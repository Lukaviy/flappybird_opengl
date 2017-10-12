#include "gui_align_panel.h"

void GuiAlignPanel_t::on_update() {
	float pos = 
		_direction == BOTTOM_TO_TOP ? _size.y :
		_direction == RIGHT_TO_LEFT ? _size.x : 0;
	for (auto element : _childs) {
		switch (_direction) {
		case TOP_TO_BOTTOM: {
			element->position(Position_t(0, pos, Position_t::ABSOLUTE));
			pos += element->bound().y;
		} break;
		case LEFT_TO_RIGHT: {
			element->position(Position_t(pos, 0, Position_t::ABSOLUTE));
			pos += element->bound().x;
		} break;
		case BOTTOM_TO_TOP: {
			element->position(Position_t(pos, 0, Position_t::ABSOLUTE));
			pos -= element->bound().x;
		} break;
		case RIGHT_TO_LEFT: {
			element->position(Position_t(0, pos, Position_t::ABSOLUTE));
			pos -= element->bound().x;
		} break;
			default: ;
		}
	}
}

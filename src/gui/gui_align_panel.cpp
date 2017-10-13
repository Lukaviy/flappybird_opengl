#include "gui_align_panel.h"

GuiAlignPanel_t::GuiAlignPanel_t(): _direction(TOP_TO_BOTTOM), _updating(false) {}

GuiAlignPanel_t* GuiAlignPanel_t::direction(Direction_t val) {
	_direction = val;
	update();
	return this;
}

GuiAlignPanel_t::Direction_t GuiAlignPanel_t::direction() const {
	return _direction;
}

void GuiAlignPanel_t::on_update() {
	if (_updating) {
		return;
	}

	_updating = true;

	GuiPanel_t::on_update();

	float pos = 0;
	for (auto element : _childs) {
		auto elem_align = element->align();
		switch (_direction) {
		case TOP_TO_BOTTOM: {
			element->align(
				elem_align == TOP_LEFT || elem_align == LEFT_BOTTOM ? TOP_LEFT :
				elem_align == TOP || elem_align == CENTER || elem_align == BOTTOM ? TOP :
				TOP_RIGHT);
			element->position(Position_t(0, pos, Position_t::ABSOLUTE));
			pos += element->bound().y;
		} break;
		case LEFT_TO_RIGHT: {
			element->align(
				elem_align == TOP_LEFT || elem_align == TOP_RIGHT || elem_align == TOP ? TOP_LEFT :
				elem_align == LEFT || elem_align == CENTER || elem_align == RIGHT ? LEFT :
				LEFT_BOTTOM);
			element->position(Position_t(pos, 0, Position_t::ABSOLUTE));
			pos += element->bound().x;
		} break;
		case BOTTOM_TO_TOP: {
			element->align(
				elem_align == TOP_LEFT || elem_align == LEFT_BOTTOM ? LEFT_BOTTOM :
				elem_align == TOP || elem_align == CENTER || elem_align == BOTTOM ? BOTTOM :
				RIGHT_BOTTOM);
			element->position(Position_t(0, pos, Position_t::ABSOLUTE));
			pos -= element->bound().y;
		} break;
		case RIGHT_TO_LEFT: {
			element->align(
				elem_align == TOP_LEFT || elem_align == TOP_RIGHT || elem_align == TOP ? TOP_RIGHT :
				elem_align == LEFT || elem_align == CENTER || elem_align == RIGHT ? RIGHT :
				RIGHT_BOTTOM);
			element->position(Position_t(pos, 0, Position_t::ABSOLUTE));
			pos -= element->bound().x;
		} break;
			default: ;
		}
	}

	_updating = false;
}

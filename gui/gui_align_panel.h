#pragma once

#include "gui_panel.h"

class GuiAlignPanel_t : public GuiPanel_t {
public:
	enum Direction_t {
		TOP_TO_BOTTOM,
		LEFT_TO_RIGHT,
		BOTTOM_TO_TOP,
		RIGHT_TO_LEFT
	};

	GuiAlignPanel_t();
protected:
	void on_update() override;

	Direction_t _direction;
};
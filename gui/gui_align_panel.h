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
	GuiAlignPanel_t* direction(Direction_t);
	Direction_t direction() const;
protected:
	void on_update() override;
	bool _updating;

	Direction_t _direction;
};
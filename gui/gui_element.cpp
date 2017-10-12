#include "gui_element.h"
#include <SFML/Graphics.hpp>

const sf::Vector2f GuiElement_t::_align_muls[] = {
	sf::Vector2f(0, 0),			//TOP_LEFT
	sf::Vector2f(.5f, 0),		//TOP
	sf::Vector2f(1, 0),			//TOP_RIGHT
	sf::Vector2f(1, .5f),		//RIGHT
	sf::Vector2f(1, 1),			//RIGHT_BOTTOM
	sf::Vector2f(.5f, 1),		//BOTTOM
	sf::Vector2f(0, 1),			//LEFT_BOTTOM
	sf::Vector2f(0, .5f),		//LEFT
	sf::Vector2f(.5f, .5f),		//CENTER
};

const GuiElement_t::Padding_t GuiElement_t::_align_margin_muls[] = {
	Padding_t(1, 1, 0, 0),		//TOP_LEFT
	Padding_t(0, 1, 0, 0),		//TOP
	Padding_t(0, 1, 1, 0),		//TOP_RIGHT
	Padding_t(0, 0, 1, 0),		//RIGHT
	Padding_t(0, 0, 1, 1),		//RIGHT_BOTTOM
	Padding_t(0, 0, 0, 1),		//BOTTOM
	Padding_t(1, 0, 0, 1),		//LEFT_BOTTOM
	Padding_t(1, 0, 0, 0),		//LEFT
	Padding_t(0, 0, 0, 0),		//CENTER
};

GuiElement_t::GuiElement_t() : _align(TOP_LEFT), _parent(nullptr), _visible(true) {}

GuiElement_t::~GuiElement_t() {
	if (_parent) {
		_parent->_childs.erase(_iter_to_self);
		_parent->update();
	}
	for (auto child : _childs) {
		child->_parent = nullptr;
		delete child;
	}
}

GuiElement_t* GuiElement_t::padding(Padding_t val) {
	_padding = val;
	update();
	return this;
}

GuiElement_t* GuiElement_t::padding(float val) {
	_padding = Padding_t(val);
	update();
	return this;
}

GuiElement_t::Padding_t GuiElement_t::padding() const {
	return _padding;
}

GuiElement_t* GuiElement_t::margin(Padding_t val) {
	_margin = val;
	update();
	return this;
}

GuiElement_t* GuiElement_t::margin(float val) {
	_margin = Padding_t(val);
	update();
	return this;
}

GuiElement_t::Padding_t GuiElement_t::margin() const {
	return _margin;
}

GuiElement_t* GuiElement_t::parent(GuiElement_t* parent) {
	if (_parent) {
		_parent->_childs.erase(_iter_to_self);
	}
	_parent = parent;
	if (_parent) {
		_iter_to_self = _parent->_childs.insert(_parent->_childs.end(), this);
	}
	update();
	return this;
}

GuiElement_t* GuiElement_t::align(Align_t val) {
	_align = val;
	update();
	return this;
}

GuiElement_t::Align_t GuiElement_t::align() const {
	return _align;
}

sf::Vector2f GuiElement_t::size() const {
	return _size;
}

GuiElement_t* GuiElement_t::size(sf::Vector2f size) {
	_size = size;
	update();
	return this;
}

GuiElement_t* GuiElement_t::size(float width, float height) {
	size(sf::Vector2f(width, height));
	return this;
}

sf::Vector2f GuiElement_t::bound() const {
	return sf::Vector2f(
		_size.x + _padding.left + _padding.right,
		_size.y + _padding.top + _padding.bottom
	);
}

bool GuiElement_t::visible() const {
	return _visible;
}

GuiElement_t* GuiElement_t::visible(bool val) {
	_visible = val;
	return this;
}

GuiElement_t* GuiElement_t::position(Position_t val) {
	_position = val;
	if (_position.type == Position_t::ABSOLUTE) {
		setPosition(_position.x, _position.y);
	} else if (_parent) {
		_parent->update();
	}
	return this;
}

GuiElement_t::Position_t GuiElement_t::position() const {
	return _position;
}

void GuiElement_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	if (_visible) {
		on_draw(target, states);
	}

	for (auto element : _childs) {
		target.draw(*element, states.transform * _align_pivots[element->align()]);
	}
}

sf::Vector2f GuiElement_t::origin_offset() const {
	return sf::Vector2f();
}

void GuiElement_t::update() {
	for (int i = 0; i < sizeof(_align_margin_muls) / sizeof(Padding_t); i++) {
		_align_pivots[i] = sf::Transform();
		auto margin = offset_by_align(_margin, _align);
		_align_pivots[i].translate(
			_size.x * _align_muls[i].x + margin.x,
			_size.y * _align_muls[i].y + margin.y
		);
	}

	on_update();

	auto align_offset = offset_by_align(_padding, _align);
	auto bound_offset = origin_offset();
	setOrigin(sf::Vector2f(
		_size.x * _align_muls[_align].x + align_offset.x + bound_offset.x,
		_size.y * _align_muls[_align].y + align_offset.y + bound_offset.y
	));

	for (auto child : _childs) {
		auto child_pos = child->position();
		if (child_pos.type == Position_t::RELATIVE) {
			child->setPosition(sf::Vector2f(
				_size.x * child_pos.x,
				_size.y * child_pos.y
			));
		}
	}

	if (_parent) {
		_parent->update();
	}
}

sf::Vector2f GuiElement_t::offset_by_align(Padding_t pad, Align_t align) {
	auto mul = _align_margin_muls[align];
	return sf::Vector2f(
		-pad.left * mul.left + pad.right * mul.right,
		-pad.top * mul.top + pad.bottom * mul.bottom
	);
}

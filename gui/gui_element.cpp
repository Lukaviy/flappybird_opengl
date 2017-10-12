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

GuiElement_t::GuiElement_t() : _align(TOP_LEFT) {}

void GuiElement_t::padding(Padding_t val) {
	_padding = val;
}

GuiElement_t::Padding_t GuiElement_t::padding() const {
	return _padding;
}

void GuiElement_t::margin(Padding_t val) {
	_margin = val;
}

GuiElement_t::Padding_t GuiElement_t::margin() const {
	return _margin;
}

void GuiElement_t::add_child(GuiElement_t& element) {
	_childs.push_back(&element);
}

void GuiElement_t::align(Align_t val) {
	_align = val;
	auto bound = size();
	
	auto offset = offset_by_align(_padding, _align);
	setOrigin(sf::Vector2f(
		bound.x * _align_muls[val].x + offset.x,
		bound.y * _align_muls[val].y + offset.y
	));
}

GuiElement_t::Align_t GuiElement_t::align() const {
	return _align;
}

void GuiElement_t::size(sf::Vector2f size) {
	_size = size;
	update();
}

sf::Vector2f GuiElement_t::bound() const {
	return sf::Vector2f(
		_size.x + _padding.left + _padding.right,
		_size.y + _padding.top + _padding.bottom
	);
}

void GuiElement_t::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	on_draw(target, states);

	for (auto element : _childs) {
		target.draw(*element, _align_pivots[element->align()]);
	}
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
}

sf::Vector2f GuiElement_t::offset_by_align(Padding_t pad, Align_t align) {
	auto mul = _align_margin_muls[align];
	return sf::Vector2f(
		pad.left * mul.left - pad.right * mul.right,
		pad.top * mul.top - pad.bottom * mul.bottom
	);
}

#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <vector>
#include <functional>
#include <list>

class GuiElement_t : public sf::Drawable, protected sf::Transformable {
public:
	GuiElement_t();
	virtual ~GuiElement_t();

	struct Padding_t {
		float left;
		float top;
		float right;
		float bottom;
		Padding_t() : left(0), top(0), right(0), bottom(0) {}
		Padding_t(float left, float top, float right, float bottom) :
			left(left), top(top), right(right), bottom(bottom) {}
	};

	GuiElement_t* padding(Padding_t val);
	Padding_t padding() const;

	GuiElement_t* margin(Padding_t val);
	Padding_t margin() const;

	GuiElement_t* set_parent(GuiElement_t* element);

	enum Align_t {
		TOP_LEFT = 0,
		TOP = 1,
		TOP_RIGHT = 2,
		RIGHT = 3,
		RIGHT_BOTTOM = 4,
		BOTTOM = 5,
		LEFT_BOTTOM = 6,
		LEFT = 7,
		CENTER = 8,
	};

	GuiElement_t* align(Align_t val);
	Align_t align() const;
	GuiElement_t* size(sf::Vector2f size);
	GuiElement_t* size(float width, float height);
	virtual sf::Vector2f size() const;
	sf::Vector2f bound() const;
	bool visible() const;
	GuiElement_t* visible(bool val);

	struct Position_t {
		enum Type_t {
			ABSOLUTE,
			RELATIVE,
		} type;
		float x;
		float y;

		Position_t() : type(ABSOLUTE), x(0), y(0) {}
		Position_t(float x, float y, Type_t type) : x(x), y(y), type(type) {}
		static Position_t relative(float x, float y) {
			return Position_t(x, y, RELATIVE);
		}
		static Position_t absolute(float x, float y) {
			return Position_t(x, y, ABSOLUTE);
		}
	};

	GuiElement_t* position(Position_t val);
	Position_t position() const;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void on_draw(sf::RenderTarget& target, sf::RenderStates states) const {};
	virtual void on_update() {}
	virtual sf::Vector2f origin_offset() const;
	void update();

	Padding_t _padding;
	Padding_t _margin;
	Align_t _align;
	sf::Vector2f _size;
	Position_t _position;
	bool _visible;

	GuiElement_t* _parent;
	std::list<GuiElement_t*> _childs;
	std::list<GuiElement_t*>::iterator _iter_to_self;

	sf::Transform _align_pivots[9];

	static sf::Vector2f offset_by_align(Padding_t pad, Align_t align);

	const static sf::Vector2f _align_muls[];
	const static Padding_t _align_margin_muls[];
};

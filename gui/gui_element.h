#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <vector>

class GuiElement_t : public sf::Drawable, sf::Transformable {
public:
	GuiElement_t();

	struct Padding_t {
		float left;
		float top;
		float right;
		float bottom;
		Padding_t() : left(0), top(0), right(0), bottom(0) {}
		Padding_t(float left, float top, float right, float bottom) :
			left(left), top(top), right(right), bottom(bottom) {}
	};

	void padding(Padding_t val);
	Padding_t padding() const;

	void margin(Padding_t val);
	Padding_t margin() const;

	void add_child(GuiElement_t& element);

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

	void align(Align_t val);
	Align_t align() const;
	virtual sf::Vector2f size() = 0;
	void size(sf::Vector2f size);
	sf::Vector2f bound() const;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void on_draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual void on_update() {}
	void update();

	Padding_t _padding;
	Padding_t _margin;
	Align_t _align;
	sf::Vector2f _size;

	std::vector<GuiElement_t*> _childs;

	sf::Transform _align_pivots[9];

	static sf::Vector2f offset_by_align(Padding_t pad, Align_t align);

	const static sf::Vector2f _align_muls[];
	const static Padding_t _align_margin_muls[];
};

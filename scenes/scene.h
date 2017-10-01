#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>

class Scene_t : public sf::Drawable, public sf::Transformable {
public:
	virtual void step(float dt) = 0;
	virtual void send_event(sf::Event event) = 0;
	virtual void reset() {};
};

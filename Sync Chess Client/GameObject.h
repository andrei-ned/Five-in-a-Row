#pragma once
#include "SFML/Graphics.hpp"

class GameObject
{
public:
	virtual void update(const sf::Time& deltaTime) {}
	virtual void render(sf::RenderWindow& window) {}
};


#pragma once
#include "SFML/Graphics.hpp"

// Base GameObject that other game objects inherit from
class GameObject
{
public:
	virtual void update(const sf::Time& deltaTime) {}
	virtual void render(sf::RenderWindow& window) {}
};


#pragma once
#include "SFML/Graphics.hpp"
#include <string>

class Game
{
public:
	Game();
	~Game();
	/// <summary> Game logic happens in here, should be called every frame </summary>
	/// <param name="deltaTime"> Time since last frame </param>
	void update(const sf::Time& deltaTime);
	// Draw things on screen, should be called every frame
	void render(sf::RenderWindow& window);
private:
	// Textures
	sf::Texture boardTexture;

	// Sprites
	sf::Sprite mBoardSprite;

	std::vector<sf::Drawable*> mDrawables;
};


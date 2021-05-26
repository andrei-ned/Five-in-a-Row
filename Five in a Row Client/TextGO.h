#pragma once
#include "GameObject.h"

class TextGO : public GameObject
{
public:
	sf::Text mText;

	virtual void render(sf::RenderWindow& window) override;
};


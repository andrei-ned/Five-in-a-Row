#pragma once
#include <string>
#include "SFML/Graphics.hpp"
#include "ButtonGO.h"

namespace Constants {
	const std::string assetPath = "../Assets/";

	const unsigned int windowWidth = 600;
	const unsigned int windowHeight = 600;
	const unsigned int playWindowWidth = 1200;
	const unsigned int playWindowHeight = 735;

	const sf::Color bgColor(0x13, 0x22, 0x2a);
	const ButtonGO::ButtonColors btnColors{
		sf::Color(0x45,0x73,0x6d),
		sf::Color(0x5c,0x99,0x91),
		sf::Color(0x48,0x6c,0x77),
	};

	const sf::Vector2f btnSize(250.0f, 50.0f);
}

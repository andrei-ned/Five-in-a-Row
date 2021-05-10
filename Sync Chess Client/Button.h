#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include <functional>

class Button : public GameObject
{
public:
	sf::Color mColorDefault;
	sf::Color mColorHovered;
	sf::Color mColorPressed;

	std::function<void()> mOnClick;

	// GameObject
	virtual void update(const sf::Time& deltaTime) override;
	virtual void render(sf::RenderWindow& window) override;

	void enable(const bool);

	void setPosition(const sf::Vector2f&);
	void setSize(const sf::Vector2f&);

	// Text
	void setTextFont(const sf::Font&);
	void setTextString(const sf::String&);
	void setTextColor(const sf::Color&);
private:
	enum class ButtonState { Default, Hovered, Pressed } mState = ButtonState::Default;
	bool mIsEnabled;
	sf::RectangleShape mRectShape;
	sf::Text mText;

	void repositionText();
};


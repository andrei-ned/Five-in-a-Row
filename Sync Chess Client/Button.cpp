#include "Button.h"
#include "SFML/Window.hpp"
#include <iostream>

void Button::update(const sf::Time& deltaTime)
{
	//std::cout << sf::Mouse::getPosition().x + " " << sf::Mouse::getPosition().y << std::endl;
	//mRectShape.getGlobalBounds().contains(sf::Mouse::getPosition())
}

void Button::render(sf::RenderWindow& window)
{
	if (mIsEnabled)
	{
		bool contains = mRectShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
		bool click = sf::Mouse::isButtonPressed(sf::Mouse::Left);

		switch (mState)
		{
		case ButtonState::Default:
			if (contains)
				mState = ButtonState::Hovered;
			break;
		case ButtonState::Hovered:
			if (!contains)
				mState = ButtonState::Default;
			if (click)
				mState = ButtonState::Pressed;
			break;
		case ButtonState::Pressed:
			if (!click)
			{
				mState = ButtonState::Hovered;
				mOnClick();
			}
			if (!contains)
				mState = ButtonState::Default;
			break;
		}

		switch (mState)
		{
		case ButtonState::Default:
			mRectShape.setFillColor(mColorDefault);
			break;
		case ButtonState::Hovered:
			mRectShape.setFillColor(mColorHovered);
			break;
		case ButtonState::Pressed:
			mRectShape.setFillColor(mColorPressed);
			break;
		}
	}


	window.draw(mRectShape);
	window.draw(mText);
}

void Button::enable(const bool enabled)
{
	mState = ButtonState::Default;
	mIsEnabled = enabled;
}

void Button::setPosition(const sf::Vector2f& pos)
{
	mRectShape.setPosition(pos);

	repositionText();
}

void Button::setSize(const sf::Vector2f& size)
{
	mRectShape.setSize(size);

	repositionText();
}

void Button::setTextFont(const sf::Font& font)
{
	mText.setFont(font);
}

void Button::setTextString(const sf::String& s)
{
	mText.setString(s);

	repositionText();
}

void Button::setTextColor(const sf::Color& color)
{
	mText.setFillColor(color);
}

void Button::repositionText()
{
	auto textRect = mText.getGlobalBounds();
	auto shapeRect = mRectShape.getGlobalBounds();

	float x = shapeRect.left + (shapeRect.width - textRect.width) * 0.5f;
	float y = shapeRect.top + (shapeRect.height - textRect.height) * 0.5f;
	y = shapeRect.top + shapeRect.height * 0.5f - textRect.height;

	mText.setPosition({ x,y });
}

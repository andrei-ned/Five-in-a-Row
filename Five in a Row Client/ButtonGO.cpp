#include "ButtonGO.h"
#include "SFML/Window.hpp"
#include <iostream>

ButtonGO::ButtonGO() : mTextOffset({ 0.0f,-5.0f }) {}

void ButtonGO::render(sf::RenderWindow& window)
{
	if (!mIsEnabled)
		return;

	if (mIsInteractable)
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
			mRectShape.setFillColor(mColors.normal);
			break;
		case ButtonState::Hovered:
			mRectShape.setFillColor(mColors.hovered);
			break;
		case ButtonState::Pressed:
			mRectShape.setFillColor(mColors.pressed);
			break;
		}
	}

	window.draw(mRectShape);
	window.draw(mText);
}

void ButtonGO::enable(bool enabled)
{
	mState = ButtonState::Default;
	mIsEnabled = enabled;
}

void ButtonGO::setInteractable(bool interact)
{
	mState = ButtonState::Default;
	mRectShape.setFillColor(mColors.normal);
	mIsInteractable = interact;
}

void ButtonGO::setPosition(const sf::Vector2f& pos)
{
	mRectShape.setPosition(pos);

	repositionText();
}

void ButtonGO::setSize(const sf::Vector2f& size)
{
	mRectShape.setSize(size);

	repositionText();
}

void ButtonGO::setColors(const ButtonColors& colors)
{
	mColors = colors;
	mRectShape.setOutlineColor(colors.outline);
}

void ButtonGO::setOutlineWidth(const float width)
{
	mRectShape.setOutlineThickness(width);
}

void ButtonGO::setTextFont(const sf::Font& font)
{
	mText.setFont(font);
}

void ButtonGO::setTextString(const sf::String& s)
{
	mText.setString(s);

	repositionText();
}

void ButtonGO::setTextColor(const sf::Color& color)
{
	mText.setFillColor(color);
}

void ButtonGO::setTextSize(const unsigned int size)
{
	mText.setCharacterSize(size);
}

void ButtonGO::repositionText()
{
	auto textRect = mText.getGlobalBounds();
	auto shapeRect = mRectShape.getGlobalBounds();

	float x = shapeRect.left + (shapeRect.width - textRect.width) * 0.5f + mTextOffset.x;
	float y = shapeRect.top + (shapeRect.height - textRect.height) * 0.5f + mTextOffset.y;

	mText.setPosition({ x,y });
}

bool ButtonGO::hasText()
{
	return !mText.getString().isEmpty();
}

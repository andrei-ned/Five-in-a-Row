#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include <functional>

class ButtonGO : public GameObject
{
public:
	struct ButtonColors {
		sf::Color normal, hovered, pressed, outline;
	};

	ButtonGO();

	std::function<void()> mOnClick;
	sf::Vector2f mTextOffset;

	// GameObject
	virtual void render(sf::RenderWindow& window) override;

	void enable(bool);
	void setInteractable(bool);

	void setPosition(const sf::Vector2f&);
	void setSize(const sf::Vector2f&);
	void setColors(const ButtonColors&);
	void setNormalColor(const sf::Color&);
	void setOutlineWidth(const float);

	// Text
	void setTextFont(const sf::Font&);
	void setTextString(const sf::String&);
	void setTextColor(const sf::Color&);
	void setTextSize(const unsigned int);
	std::string getText() const;
	bool hasText() const;
private:
	enum class ButtonState { Default, Hovered, Pressed } mState = ButtonState::Default;
	ButtonColors mColors;
	bool mIsEnabled = true;
	bool mIsInteractable = true;
	sf::RectangleShape mRectShape;
	sf::Text mText;

	void repositionText();
};


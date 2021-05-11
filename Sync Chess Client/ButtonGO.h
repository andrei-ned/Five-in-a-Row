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

	std::function<void()> mOnClick;
	float textYOffset = -5.0f;

	// GameObject
	virtual void update(const sf::Time& deltaTime) override;
	virtual void render(sf::RenderWindow& window) override;

	void enable(const bool);

	void setPosition(const sf::Vector2f&);
	void setSize(const sf::Vector2f&);
	void setColors(const ButtonColors&);
	void setOutlineWidth(const float);

	// Text
	void setTextFont(const sf::Font&);
	void setTextString(const sf::String&);
	void setTextColor(const sf::Color&);
	void setTextSize(const unsigned int);
private:
	enum class ButtonState { Default, Hovered, Pressed } mState = ButtonState::Default;
	ButtonColors mColors;
	bool mIsEnabled = true;
	sf::RectangleShape mRectShape;
	sf::Text mText;

	void repositionText();
};


#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
using namespace sf;

class Button : public Drawable, public Transformable
{
	RectangleShape button;
	std::string text1, text2;
	Text label;
	virtual void draw(RenderTarget& target, RenderStates states) const;

public:
	Button(Vector2f position, float width, float height, std::string text1, std::string text2, Font& font, int fontSize, Color fill, Color outline);
	void ButtonPressed();
	void move(float offsetX, float offsetY);
	bool contains(Vector2f point);
};
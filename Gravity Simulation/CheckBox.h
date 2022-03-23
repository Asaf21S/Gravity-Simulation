#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
using namespace sf;

class CheckBox : public Drawable, public Transformable
{
	RectangleShape button;
	Text label;
	bool checked;
	virtual void draw(RenderTarget& target, RenderStates states) const;

public:
	CheckBox(float yPosition, std::string text, Font& font, bool isChecked);
	void Clicked();
	bool IsChecked();
	bool contains(Vector2f point);
};
#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
using namespace sf;

class CheckBox : public Drawable, public Transformable
{
	RectangleShape button;
	Font font;
	Text label;
	bool isClicked;
	virtual void draw(RenderTarget& target, RenderStates states) const;

public:
	CheckBox(Vector2f position, std::string text);
	void Update(int mouseX, int mouseY);

	bool checked;
};
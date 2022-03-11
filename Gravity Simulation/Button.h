#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
using namespace sf;

class Button : public Drawable, public Transformable
{
	RectangleShape button;
	Font font;
	Text label;
	void (*CallBack)();
	bool isClicked;
	virtual void draw(RenderTarget& target, RenderStates states) const;

public:
	Button(Vector2f position, float width, float height, void (*f)(), std::string text, int fontSize, Color fill, Color outline);
	void Update(int mouseX, int mouseY);
	void SetText(std::string text);
	void move(float offsetX, float offsetY);
	bool contains(Vector2f point);
};
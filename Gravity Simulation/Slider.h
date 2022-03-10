#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
using namespace sf;

class Slider : public Drawable, public Transformable
{
	CircleShape slider;
	RectangleShape axis;
	Font font;
	int minValue;
	int maxValue;
	float sliderValue;
	bool isClicked;
	bool modify;
	Text GetText(float x, float y, std::string z, int fontSize) const;
	virtual void draw(RenderTarget& target, RenderStates states) const;

public:
	Slider(Vector2f position, float minValue, float maxValue, float initialValue);
	void Update(RenderWindow& window);
	float GetValue();
	void SetValue(float newValue);
	void SetPercentValue(float newPercentValue);
};
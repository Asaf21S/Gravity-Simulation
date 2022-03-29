#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <format>
using namespace sf;

class Slider : public Drawable, public Transformable
{
	CircleShape slider;
	RectangleShape axis;
	std::string label;
	Font font;
	int minValue;
	int maxValue;
	float sliderValue;
	bool modify, showFloatingPoint;
	Text GetText(float x, float y, std::string z, int fontSize) const;
	virtual void draw(RenderTarget& target, RenderStates states) const;

public:
	Slider(Vector2f position, float minValue, float maxValue, float initialValue, std::string label, Font& font);
	void Update(float mouseX);
	void SetModify(bool mod);
	bool GetModify();
	float GetValue();
	void SetValue(float newValue);
	void SetPercentValue(float newPercentValue);
	bool contains(Vector2f point);
};
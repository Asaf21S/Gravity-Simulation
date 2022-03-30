#include "Slider.h"

/**
	Slider constructor.
	@param position - the position of the center of the slider.
	@param minValue - the minimum value of the slider.
	@param maxValue - the maximum value of the slider.
	@param initialValue - the initial value of the slider.
	@param label - the text to display above the slider.
	@param font - the font family of the text.
*/
Slider::Slider(Vector2f position, float minValue, float maxValue, float initialValue, std::string label, Font& font) :
	axis(Vector2f(200, 10)),
	label(label),
	font(font),
	slider(12),
	minValue(minValue),
	maxValue(maxValue),
	modify(false),
	showFloatingPoint(maxValue - minValue < 10)
{
	axis.setOrigin(axis.getLocalBounds().left + axis.getLocalBounds().width / 2.0f, axis.getLocalBounds().top + axis.getLocalBounds().height / 2.0f);
	axis.setPosition(position);
	axis.setFillColor(Color(224, 224, 224));

	slider.setOrigin(Vector2f(slider.getRadius(), slider.getRadius()));
	slider.setPosition(position.x - axis.getGlobalBounds().width / 2.0f, position.y);
	slider.setFillColor(Color(102, 0, 204));
	slider.setOutlineColor(Color(204, 153, 255));
	slider.setOutlineThickness(-4);

	SetValue(initialValue);
}

/**
	Creating an sf::Text object.
	@param x - the x component of the text position.
	@param y - the y component of the text position.
	@param z - the text to display.
	@param fontSize - the size of the text.
	@return the Text object.
*/
Text Slider::GetText(float x, float y, std::string z, int fontSize) const
{
	Text t(z, font, fontSize);
	t.setFillColor(Color::White);
	t.setOrigin(t.getLocalBounds().left + t.getLocalBounds().width / 2.0f, t.getLocalBounds().top + t.getLocalBounds().height / 2.0f);
	t.setPosition(x, y);
	return t;
}

/**
	When modify is true, the slider is moving according to the mouse position.
	@param mod - the value which modify will be set to.
*/
void Slider::SetModify(bool mod)
{
	modify = mod;
}

/**
	When modify is true, the slider is moving according to the mouse position.
	@return modify.
*/
bool Slider::GetModify()
{
	return modify;
}

/**
	Updating the slider position according to the mouse position. 
	If the mouse position is to the left of the slider, the value will be set to the minimum value. 
	And if the it's to the right of the slider, value will be set to the maximum value.
	@param mouseX - the x component of the mouse position.
*/
void Slider::Update(float mouseX)
{
	if (mouseX < axis.getGlobalBounds().left)
	{
		slider.setPosition(axis.getGlobalBounds().left, axis.getPosition().y);
		sliderValue = minValue;
	}
	else if (mouseX > axis.getGlobalBounds().left + axis.getSize().x)
	{
		slider.setPosition(axis.getGlobalBounds().left + axis.getSize().x, axis.getPosition().y);
		sliderValue = maxValue;
	}
	else
	{
		slider.setPosition(mouseX, axis.getPosition().y);
		sliderValue = (minValue + ((slider.getPosition().x - axis.getGlobalBounds().left) / axis.getGlobalBounds().width * (maxValue - minValue)));
	}
}

/**
	Get the current value of the slider.
	@return sliderValue.
*/
float Slider::GetValue()
{
	return sliderValue;
}

/**
	Set the value of the slider. 
	If the mouse position is to the left of the slider, the value will be set to the minimum value. 
	And if the it's to the right of the slider, value will be set to the maximum value.
	@param newValue - the value which the slider will be set to.
*/
void Slider::SetValue(float newValue)
{
	if (newValue < minValue)
	{
		slider.setPosition(axis.getGlobalBounds().left, axis.getPosition().y);
		sliderValue = minValue;
	}
	else if (newValue > maxValue)
	{
		slider.setPosition(axis.getGlobalBounds().left + axis.getSize().x, axis.getPosition().y);
		sliderValue = maxValue;
	}
	else
	{
		slider.setPosition(axis.getGlobalBounds().left + ((newValue - minValue) / (maxValue - minValue)) * axis.getSize().x, axis.getPosition().y);
		sliderValue = newValue;
	}
}

/**
	Set the value of the slider in percentage.
	@param newPercentValue - the percentage which the slider will be set to.
*/
void Slider::SetPercentValue(float newPercentValue)
{
	if (newPercentValue >= 0 && newPercentValue <= 100)
	{
		sliderValue = newPercentValue / 100 * maxValue;
		slider.setPosition(axis.getGlobalBounds().left + (axis.getSize().x * newPercentValue / 100), axis.getPosition().y);
	}
}

/**
	Drawing the slider.
*/
void Slider::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = NULL;
	target.draw(GetText(axis.getGlobalBounds().left + axis.getSize().x / 2, axis.getPosition().y - 4.5 * axis.getLocalBounds().height, label, 24), states);
	target.draw(GetText(axis.getGlobalBounds().left, axis.getPosition().y + 2 * axis.getLocalBounds().height, std::to_string(minValue), 20), states);
	target.draw(axis, states);
	target.draw(GetText(axis.getGlobalBounds().left + axis.getSize().x, axis.getPosition().y + 2 * axis.getLocalBounds().height, std::to_string(maxValue), 20), states);
	target.draw(slider, states);
	std::string val = showFloatingPoint ? std::to_string(sliderValue).substr(0, std::to_string(sliderValue).find(".") + 3) : std::to_string((int)sliderValue);
	target.draw(GetText(slider.getPosition().x, slider.getPosition().y - 2 * slider.getRadius(), val, 15), states);
}

/**
	Check if a point is inside the circle of the slider.
	@param point - the point to check for.
	@return true if the point is inside the circle of the slider or false otherwise.
*/
bool Slider::contains(Vector2f point)
{
	return std::hypotf(slider.getPosition().x - point.x, slider.getPosition().y - point.y) <= slider.getRadius();
}
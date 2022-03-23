#include "Slider.h"

Slider::Slider(Vector2f position, float minValue, float maxValue, float initialValue, Font& font) :
	axis(Vector2f(200, 10)),
	font(font),
	slider(12),
	minValue(minValue),
	maxValue(maxValue),
	modify(false)
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

Text Slider::GetText(float x, float y, std::string z, int fontSize) const
{
	Text t(z, font, fontSize);
	t.setFillColor(Color::White);
	t.setOrigin(t.getLocalBounds().left + t.getLocalBounds().width / 2.0f, t.getLocalBounds().top + t.getLocalBounds().height / 2.0f);
	t.setPosition(x, y);
	return t;
}

void Slider::SetModify(bool mod)
{
	modify = mod;
}

bool Slider::GetModify()
{
	return modify;
}

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

float Slider::GetValue()
{
	return sliderValue;
}

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

void Slider::SetPercentValue(float newPercentValue)
{
	if (newPercentValue >= 0 && newPercentValue <= 100)
	{
		sliderValue = newPercentValue / 100 * maxValue;
		slider.setPosition(axis.getGlobalBounds().left + (axis.getSize().x * newPercentValue / 100), axis.getPosition().y);
	}
}

void Slider::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = NULL;
	target.draw(GetText(axis.getGlobalBounds().left, axis.getPosition().y + 2 * axis.getLocalBounds().height, std::to_string(minValue), 20));
	target.draw(axis);
	target.draw(GetText(axis.getGlobalBounds().left + axis.getSize().x, axis.getPosition().y + 2 * axis.getLocalBounds().height, std::to_string(maxValue), 20));
	target.draw(slider);
	target.draw(GetText(slider.getPosition().x, slider.getPosition().y - 2 * slider.getRadius(), std::to_string((int)sliderValue), 15));
}

bool Slider::contains(Vector2f point)
{
	return slider.getGlobalBounds().contains(point);
}
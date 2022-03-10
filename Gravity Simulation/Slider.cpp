#include "Slider.h"

Slider::Slider(Vector2f position, float minValue, float maxValue, float initialValue) :
	axis(Vector2f(200, 10)),
	slider(12),
	minValue(minValue),
	maxValue(maxValue),
	modify(false)
{
	axis.setPosition(position);
	axis.setOrigin(0, 5);
	axis.setFillColor(Color(63, 63, 63));

	slider.setOrigin(Vector2f(slider.getRadius(), slider.getRadius()));
	slider.setPosition(position);
	slider.setFillColor(Color(102, 0, 204));
	slider.setOutlineColor(Color(204, 153, 255));
	slider.setOutlineThickness(-4);

	if (!font.loadFromFile("Fonts\\Neon.ttf"))
		std::cout << "Error loading font\n";

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

void Slider::Update(RenderWindow& window)
{
	if (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		if (slider.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y) && isClicked == false)
			modify = true;
		isClicked = true;
	}
	else
	{
		modify = false;
		isClicked = false;
	}

	if (modify)
	{
		if (Mouse::getPosition(window).x < axis.getPosition().x)
		{
			slider.setPosition(axis.getPosition().x, axis.getPosition().y);
			sliderValue = minValue;
		}
		else if (Mouse::getPosition(window).x > axis.getPosition().x + axis.getSize().x)
		{
			slider.setPosition(axis.getPosition().x + axis.getSize().x, axis.getPosition().y);
			sliderValue = maxValue;
		}
		else
		{
			slider.setPosition(Mouse::getPosition(window).x, axis.getPosition().y);
			sliderValue = (minValue + ((slider.getPosition().x - axis.getPosition().x) / axis.getSize().x * (maxValue - minValue)));
		}
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
		slider.setPosition(axis.getPosition().x, axis.getPosition().y);
		sliderValue = minValue;
	}
	else if (newValue > maxValue)
	{
		slider.setPosition(axis.getPosition().x + axis.getSize().x, axis.getPosition().y);
		sliderValue = maxValue;
	}
	else
	{
		slider.setPosition(axis.getPosition().x + ((newValue - minValue) / (maxValue - minValue)) * axis.getSize().x, axis.getPosition().y);
		sliderValue = newValue;
	}
}

void Slider::SetPercentValue(float newPercentValue)
{
	if (newPercentValue >= 0 && newPercentValue <= 100)
	{
		sliderValue = newPercentValue / 100 * maxValue;
		slider.setPosition(axis.getPosition().x + (axis.getSize().x * newPercentValue / 100), axis.getPosition().y);
	}
}

void Slider::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = NULL;
	target.draw(GetText(axis.getPosition().x, axis.getPosition().y + 2 * axis.getLocalBounds().height, std::to_string(minValue), 20));
	target.draw(axis);
	target.draw(GetText(axis.getPosition().x + axis.getSize().x, axis.getPosition().y + 2 * axis.getLocalBounds().height, std::to_string(maxValue), 20));
	target.draw(slider);
	target.draw(GetText(slider.getPosition().x, slider.getPosition().y - 2 * slider.getRadius(), std::to_string((int)sliderValue), 15));
}
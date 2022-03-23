#include "Button.h"

Button::Button(Vector2f position, float width, float height, std::string text1, std::string text2, Font& font, int fontSize, Color fill, Color outline) :
	button(Vector2f(width, height)),
	text1(text1),
	text2(text2),
	label(text1, font, fontSize)
{
	button.setPosition(position);
	button.setOrigin(button.getLocalBounds().left + button.getLocalBounds().width / 2.0f, button.getLocalBounds().top + button.getLocalBounds().height / 2.0f);
	button.setFillColor(fill);
	button.setOutlineThickness(5);
	button.setOutlineColor(outline);

	label.setFillColor(Color::Black);
	label.setString(text1);
	label.setOrigin(label.getLocalBounds().left + label.getLocalBounds().width / 2.0f, label.getLocalBounds().top + label.getLocalBounds().height / 2.0f);
	label.setPosition(button.getPosition());
}

void Button::ButtonPressed()
{
	label.setString(label.getString() == text1 ? text2 : text1);
	label.setOrigin(label.getLocalBounds().left + label.getLocalBounds().width / 2.0f, label.getLocalBounds().top + label.getLocalBounds().height / 2.0f);
	label.setPosition(button.getPosition());
}

void Button::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = NULL;
	states.transform *= getTransform();

	target.draw(button, states);
	target.draw(label, states);
}

void Button::move(float offsetX, float offsetY)
{
	button.move(Vector2f(offsetX, offsetY));
	label.move(Vector2f(offsetX, offsetY));
}

bool Button::contains(Vector2f point)
{
	return button.getGlobalBounds().contains(point);
}

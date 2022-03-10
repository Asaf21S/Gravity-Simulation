#include "Button.h"

Button::Button(Vector2f position, float width, float height, void (*CallBack)(), std::string text, Color fill, Color outline) :
	button(Vector2f(width, height)),
	label(text, font, 32),
	isClicked(false),
	CallBack(CallBack)
{
	button.setPosition(position);
	button.setOrigin(button.getLocalBounds().left + button.getLocalBounds().width / 2.0f, button.getLocalBounds().top + button.getLocalBounds().height / 2.0f);
	button.setFillColor(fill);
	button.setOutlineThickness(5);
	button.setOutlineColor(outline);

	if (!font.loadFromFile("Fonts\\Neon.ttf"))
		std::cout << "Error loading font\n";

	label.setFillColor(Color::Black);
	label.setOrigin(label.getLocalBounds().left + label.getLocalBounds().width / 2.0f, label.getLocalBounds().top + label.getLocalBounds().height / 2.0f);
	label.setPosition(button.getPosition());
}

void Button::Update(RenderWindow& window)
{
	if (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		if (button.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y) && isClicked == false)
			CallBack();
		isClicked = true;
	}
	else isClicked = false;
}

void Button::SetText(std::string text)
{
	label.setString(text);
}

void Button::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = NULL;

	target.draw(button);
	target.draw(label);
}
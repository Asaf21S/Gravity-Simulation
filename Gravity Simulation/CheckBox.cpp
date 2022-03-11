#include "CheckBox.h"

CheckBox::CheckBox(Vector2f position, std::string text) :
	button(Vector2f(30, 30)),
	label(text, font, 24),
	checked(false),
	isClicked(false)
{
	button.setPosition(position);
	button.setOrigin(button.getLocalBounds().left + button.getLocalBounds().width / 2.0f, button.getLocalBounds().top + button.getLocalBounds().height / 2.0f);
	button.setFillColor(Color::Black);
	button.setOutlineThickness(5);
	button.setOutlineColor(Color(63, 63, 63));

	if (!font.loadFromFile("Fonts\\Neon.ttf"))
		std::cout << "Error loading font\n";

	label.setFillColor(Color::White);
	label.setOrigin(0, label.getLocalBounds().top + label.getLocalBounds().height / 2.0f);
	label.setPosition(button.getPosition().x + button.getLocalBounds().width, button.getPosition().y);
}

void CheckBox::Update(int mouseX, int mouseY)
{
	if (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		if (button.getGlobalBounds().contains(mouseX, mouseY) && isClicked == false)
			checked = !checked;
		isClicked = true;
	}
	else isClicked = false;

	if (checked)
		button.setFillColor(Color::Blue);
	else
		button.setFillColor(Color::Black);
}

void CheckBox::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = NULL;

	target.draw(button);
	target.draw(label);
}
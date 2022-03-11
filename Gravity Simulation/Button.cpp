#include "Button.h"

Button::Button(Vector2f position, float width, float height, void (*CallBack)(), std::string text, int fontSize, Color fill, Color outline) :
	button(Vector2f(width, height)),
	label(text, font, fontSize),
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
	SetText(text);
}

void Button::Update(int mouseX, int mouseY)
{
	if (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		if (button.getGlobalBounds().contains(mouseX, mouseY) && isClicked == false)
			CallBack();
		isClicked = true;
	}
	else isClicked = false;
}

void Button::SetText(std::string text)
{
	label.setString(text);
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

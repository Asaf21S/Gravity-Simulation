#include "Button.h"

/**
	Button constructor.
	@param position - the position of the center of the button.
	@param width - button's width.
	@param height - button's height.
	@param text1 - first text that displayed on the button.
	@param text2 - second text that displayed after the button is clicked.
	@param font - the font family of the text.
	@param fontSize - the size of the text.
	@param fill - button's color.
	@param outline - button's outline color.
*/
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

/**
	Changing the text from text1 to text2 or vice versa.
*/
void Button::ButtonPressed()
{
	label.setString(label.getString() == text1 ? text2 : text1);
	label.setOrigin(label.getLocalBounds().left + label.getLocalBounds().width / 2.0f, label.getLocalBounds().top + label.getLocalBounds().height / 2.0f);
	label.setPosition(button.getPosition());
}

/**
	Drawing the button.
*/
void Button::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = NULL;
	states.transform *= getTransform();

	target.draw(button, states);
	target.draw(label, states);
}

/**
	Move the button.
	@param offsetX - offset to move on X axis.
	@param offsetY - offset to move on Y axis.
*/
void Button::move(float offsetX, float offsetY)
{
	button.move(Vector2f(offsetX, offsetY));
	label.move(Vector2f(offsetX, offsetY));
}

/**
	Check if a point is inside the button.
	@param point - the point to check for.
	@return true if the point is inside the button or false otherwise.
*/
bool Button::contains(Vector2f point)
{
	return button.getGlobalBounds().contains(point);
}

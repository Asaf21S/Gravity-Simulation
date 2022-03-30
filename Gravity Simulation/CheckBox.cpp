#include "CheckBox.h"

/**
	CheckBox constructor.
	@param yPosition - the y component of the checkbox position.
	@param text - the text to display next to the checkbox.
	@param font - the font family of the text.
	@param isChecked - whether the checkbox is initially checked or not.
*/
CheckBox::CheckBox(float yPosition, std::string text, Font& font, bool isChecked) :
	button(Vector2f(30, 30)),
	label(text, font, 24),
	checked(isChecked)
{
	button.setOrigin(button.getLocalBounds().left + button.getLocalBounds().width / 2.0f, button.getLocalBounds().top + button.getLocalBounds().height / 2.0f);
	button.setPosition(button.getLocalBounds().width / 2.0f + 20, yPosition);
	button.setFillColor(Color::Black);
	button.setOutlineThickness(5);
	button.setOutlineColor(Color(63, 63, 63));

	if (checked)
		button.setFillColor(Color::Blue);
	else
		button.setFillColor(Color::Black);

	label.setFillColor(Color::White);
	label.setOrigin(0, label.getLocalBounds().top + label.getLocalBounds().height / 2.0f);
	label.setPosition(button.getPosition().x + button.getLocalBounds().width, button.getPosition().y);
}

/**
	Check/Uncheck.
	Changing the color of the checkbox accordingly.
*/
void CheckBox::Clicked()
{
	checked = !checked;
	if (checked)
		button.setFillColor(Color::Blue);
	else
		button.setFillColor(Color::Black);
}

/**
	@return true if the checkbox is currently checked or false otherwise.
*/
bool CheckBox::IsChecked()
{
	return checked;
}

/**
	Drawing the checkbox.
*/
void CheckBox::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = NULL;

	target.draw(button);
	target.draw(label);
}

/**
	Check if a point is inside the checkbox.
	@param point - the point to check for.
	@return true if the point is inside the checkbox or false otherwise.
*/
bool CheckBox::contains(Vector2f point)
{
	return button.getGlobalBounds().contains(point);
}
#include "Menu.h"

Menu::Menu(Font& font) :
	btnCollapseMenu(Vector2f(MENU_WIDTH + 6 + COLLAPSE_MENU_WIDTH / 2.0f + 5, COLLAPSE_MENU_WIDTH / 2.0f + 5), COLLAPSE_MENU_WIDTH, COLLAPSE_MENU_WIDTH, "<", ">", font, 64, Color(128, 128, 128, 200), Color(128, 128, 128, 200)),
	slConstG(Vector2f(MENU_WIDTH / 2, 150), 0, 100, 50, font),
	cbVel(250, "Show Planet's Velocity", font, true),
	cbAcc(350, "Show Planet's Acceleration", font, false),
	btnState(Vector2f(MENU_WIDTH / 2, 450), 100, 50, "Start", "Pause", font, 32, Color::White, Color::Magenta),
	isCollapsed(false),
	showMenuItems(true)
{
	menuBackground.setFillColor(Color(32, 32, 32, 200));
	menuBackground.setOutlineColor(Color(128, 128, 128, 200));
	menuBackground.setOutlineThickness(6);
	
	menuTitle.setFont(font);
	menuTitle.setString("Menu");
	menuTitle.setCharacterSize(64);
	menuTitle.setStyle(Text::Style::Underlined);
	menuTitle.setOrigin(menuTitle.getLocalBounds().left + menuTitle.getLocalBounds().width / 2.0f, menuTitle.getLocalBounds().top + menuTitle.getLocalBounds().height / 2.0f);
	menuTitle.setPosition(MENU_WIDTH / 2, 15 + menuTitle.getLocalBounds().height / 2.0f);
}

void Menu::Init(PlanetSystem& sys, int height)
{
	if (cbVel.IsChecked()) sys.SetVelVisibility();
	if (cbAcc.IsChecked()) sys.SetAccVisibility();
	menuBackground.setSize(Vector2f(MENU_WIDTH, height));
}

bool Menu::IsClickInside(Vector2f mouse)
{
	return menuBackground.getGlobalBounds().contains(mouse) || btnCollapseMenu.contains(mouse);
}

void Menu::MouseClicked(Vector2f mousePos, PlanetSystem& sys)
{
	if (btnCollapseMenu.contains(mousePos))
	{
		isCollapsed = !isCollapsed;
		if (isCollapsed)
		{
			showMenuItems = false;
			menuBackground.setSize(Vector2f(menuBackground.getSize().x - 40, menuBackground.getSize().y));
			btnCollapseMenu.move(-40.0f, 0);
		}
		else
		{
			menuBackground.setSize(Vector2f(menuBackground.getSize().x + 40, menuBackground.getSize().y));
			btnCollapseMenu.move(40.0f, 0);
		}
	}
	else if (!showMenuItems)
	{
		// if the following items are hidden, don't continue to check if we clicked on them
	}
	else if (slConstG.contains(mousePos))
	{
		slConstG.SetModify(true);
	}
	else if (btnState.contains(mousePos))
	{
		btnState.ButtonPressed();
		sys.SetState();
	}
	else if (cbVel.contains(mousePos))
	{
		cbVel.Clicked();
		sys.SetVelVisibility();
	}
	else if (cbAcc.contains(mousePos))
	{
		cbAcc.Clicked();
		sys.SetAccVisibility();
	}
}

void Menu::MouseReleased()
{
	slConstG.SetModify(false);
}

void Menu::UpdateSlider(float mouseX, PlanetSystem& sys)
{
	slConstG.Update(mouseX);
	sys.SetGConst(slConstG.GetValue());
}

bool Menu::TrackMouse()
{
	return slConstG.GetModify();
}

bool Menu::MenuIsCollapsed()
{
	return (isCollapsed && menuBackground.getSize().x > 0) || (!isCollapsed && menuBackground.getSize().x < MENU_WIDTH);
}

void Menu::CollapseMenu()
{
	if (isCollapsed)
	{
		if (menuBackground.getSize().x > 40)
		{
			menuBackground.setSize(Vector2f(menuBackground.getSize().x - 40, menuBackground.getSize().y));
			btnCollapseMenu.move(-40.0f, 0);
		}
		else if (menuBackground.getSize().x > 0)
		{
			btnCollapseMenu.move(-menuBackground.getSize().x, 0);
			menuBackground.setSize(Vector2f(0, menuBackground.getSize().y));
			btnCollapseMenu.ButtonPressed(); // only to change the text to ">"
		}
	}
	else
	{
		if (menuBackground.getSize().x < MENU_WIDTH - 40)
		{
			menuBackground.setSize(Vector2f(menuBackground.getSize().x + 40, menuBackground.getSize().y));
			btnCollapseMenu.move(40.0f, 0);
		}
		else if (menuBackground.getSize().x < MENU_WIDTH)
		{
			showMenuItems = true;
			btnCollapseMenu.move(MENU_WIDTH - menuBackground.getSize().x, 0);
			menuBackground.setSize(Vector2f(MENU_WIDTH, menuBackground.getSize().y));
			btnCollapseMenu.ButtonPressed(); // only to change the text to "<"
		}
	}
}

void Menu::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = NULL;

	target.draw(menuBackground, states);
	target.draw(btnCollapseMenu, states);

	if (showMenuItems)
	{
		target.draw(menuTitle, states);
		target.draw(slConstG, states);
		target.draw(btnState, states);
		target.draw(cbVel, states);
		target.draw(cbAcc, states);
	}
}
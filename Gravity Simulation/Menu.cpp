﻿#include "Menu.h"

/**
	Menu constructor.
	@param font - the font family of the texts.
*/
Menu::Menu(Font& font) :
	btnCollapseMenu(Vector2f(MENU_WIDTH + 6 + COLLAPSE_MENU_WIDTH / 2.0f + 5, COLLAPSE_MENU_WIDTH / 2.0f + 5), COLLAPSE_MENU_WIDTH, COLLAPSE_MENU_WIDTH, "<", ">", font, 64, Color(128, 128, 128, 200), Color(128, 128, 128, 200)),
	btnSwitchMenu(Vector2f(MENU_WIDTH / 2, 120), 300, 40, "Edit Planets", "Back to Main Menu", font, 32, Color::Cyan, Color::White),
	isCollapsed(false),
	showMenuItems(true),
	planetIndex(-1),
	currentSlider(MenuSlider::none),
	slConstG(Vector2f(MENU_WIDTH / 2, 240), 0, 100, 50, "Gravitational Constant", font),
	cbVel(340, "Show Planet's Velocity", font, true),
	cbAcc(400, "Show Planet's Acceleration", font, false),
	btnClear(Vector2f(MENU_WIDTH / 2, 480), 240, 50, "Remove Everything", "Remove Everything", font, 28, Color::White, Color::Red),
	btnState(Vector2f(MENU_WIDTH / 2, 580), 100, 50, "Start", "Pause", font, 28, Color::White, Color::Magenta),
	btnPreviousPlanet(Vector2f(MENU_WIDTH / 2 - 40, 200), 50, 35, "<-", "<-", font, 64, Color(204, 255, 255), Color(102, 102, 255)),
	btnNextPlanet(Vector2f(MENU_WIDTH / 2 + 40, 200), 50, 35, "->", "->", font, 64, Color(204, 255, 255), Color(102, 102, 255)),
	slPlanetSize(Vector2f(MENU_WIDTH / 2, 320), 10, 500, 10, "Planet Size", font),
	slPlanetDensity(Vector2f(MENU_WIDTH / 2, 420), 0.5, 6, 1, "Planet Density", font),
	slPlanetVelDirection(Vector2f(MENU_WIDTH / 2, 560), 0, 360, 0, "Velocity Direction", font),
	slPlanetVelMagnitude(Vector2f(MENU_WIDTH / 2, 660), 0, 5, 0, "Velocity Magnitude", font),
	btnPlanetDelete(Vector2f(MENU_WIDTH / 2, 780), 250, 50, "Remove Planet", "Remove Planet", font, 28, Color::White, Color::Red)
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

	stats.setFont(font);
	stats.setString("Amount of planets: 0\nYears passed: 0");
	stats.setCharacterSize(28);
	stats.setOrigin(stats.getLocalBounds().left + stats.getLocalBounds().width / 2.0f, stats.getLocalBounds().top + stats.getLocalBounds().height / 2.0f);
}

/**
	Initiate the menu after the window has been created.
	@param sys - the planet system object to update the arrows visibility to.
	@param height - the height of the window.
*/
void Menu::Init(PlanetSystem& sys, int height)
{
	if (cbVel.IsChecked()) sys.ToggleArrowVisibility(true);
	if (cbAcc.IsChecked()) sys.ToggleArrowVisibility(false);
	menuBackground.setSize(Vector2f(MENU_WIDTH, height));
	stats.setPosition(MENU_WIDTH / 2, height - 200);
}

/**
	check whether the mouse position is inside the menu area.
	@param mousePos - the position of the mouse.
*/
bool Menu::IsClickInside(Vector2f mousePos)
{
	return menuBackground.getGlobalBounds().contains(mousePos) || btnCollapseMenu.contains(mousePos);
}

/**
	Called when the mouse is clicked on the menu. If it's inside any menu item, it will trigger its event.
	@param mousePos - the position of the mouse.
	@param sys - the planet system object to update on menu item's events.
*/
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
	else if (showMenuItems) // if menu isn't collapsed
	{
		if (btnSwitchMenu.contains(mousePos) && sys.GetAmount() > 0)
		{
			SwitchMenus(sys);
		}
		else if (planetIndex == -1) // Main menu items:
		{
			if (slConstG.contains(mousePos))
			{
				slConstG.SetModify(true);
				currentSlider = MenuSlider::constG;
			}
			else if (cbVel.contains(mousePos))
			{
				cbVel.Clicked();
				sys.ToggleArrowVisibility(true);
			}
			else if (cbAcc.contains(mousePos))
			{
				cbAcc.Clicked();
				sys.ToggleArrowVisibility(false);
			}
			else if (btnClear.contains(mousePos))
			{
				sys.ClearEverything();
			}
			else if (btnState.contains(mousePos))
			{
				btnState.ButtonPressed();
				sys.ToggleState();
			}
		}
		else // planet menu items:
		{
			if (btnPreviousPlanet.contains(mousePos))
			{
				planetIndex = (planetIndex - 1 + sys.GetAmount()) % sys.GetAmount();
				EditPlanet(sys.Editing(planetIndex));
			}
			else if (btnNextPlanet.contains(mousePos))
			{
				planetIndex = (planetIndex + 1 + sys.GetAmount()) % sys.GetAmount();
				EditPlanet(sys.Editing(planetIndex));
			}
			else if (slPlanetSize.contains(mousePos))
			{
				slPlanetSize.SetModify(true);
				currentSlider = MenuSlider::pSize;
			}
			else if (slPlanetDensity.contains(mousePos))
			{
				slPlanetDensity.SetModify(true);
				currentSlider = MenuSlider::pDensity;
			}
			else if (slPlanetVelDirection.contains(mousePos))
			{
				slPlanetVelDirection.SetModify(true);
				currentSlider = MenuSlider::pVelDir;
			}
			else if (slPlanetVelMagnitude.contains(mousePos))
			{
				slPlanetVelMagnitude.SetModify(true);
				currentSlider = MenuSlider::pVelMag;
			}
			else if (btnPlanetDelete.contains(mousePos))
			{
				sys.RemovePlanet(planetIndex);
				SwitchMenus(sys);
			}
		}
	}
}

/**
	Called when the mouse is released.
*/
void Menu::MouseReleased()
{
	switch (currentSlider)
	{
	case MenuSlider::constG:
		slConstG.SetModify(false);
		break;
	case MenuSlider::pSize:
		slPlanetSize.SetModify(false);
		break;
	case MenuSlider::pDensity:
		slPlanetDensity.SetModify(false);
		break;
	case MenuSlider::pVelDir:
		slPlanetVelDirection.SetModify(false);
		break;
	case MenuSlider::pVelMag:
		slPlanetVelMagnitude.SetModify(false);
		break;
	default:
		break;
	}
	currentSlider = MenuSlider::none;
}

/**
	Switch between the main menu and the 'edit planet' menu.
	@param sys - the planet system object to update the planets outline.
	@param pInd - if pInd >= 0, edit the planet at index pInd. If pInd == -1, toggle between the main menu and editing the planet at index 0.
*/
void Menu::SwitchMenus(PlanetSystem& sys, int pInd)
{
	if (planetIndex == -1) // currently on the main menu
	{
		btnSwitchMenu.ButtonPressed();
		if (pInd == -1) planetIndex = 0; // button is clicked
		else planetIndex = pInd; // planet is clicked
		EditPlanet(sys.Editing(planetIndex));
	}
	else // currently on the 'edit planet' menu
	{
		if (pInd == -1) // switch to the main menu
		{
			btnSwitchMenu.ButtonPressed();
			planetIndex = -1;
			sys.RemoveOutlines();
		}
		else // edit a different planet
		{
			planetIndex = pInd; // planet is clicked
			EditPlanet(sys.Editing(planetIndex));
		}
	}
}

/**
	Fill the 'edit planet' menu items with the attributes of a specific planet.
	@param p - the planet that is being edited.
*/
void Menu::EditPlanet(const Planet& p)
{
	slPlanetSize.SetValue(p.GetRadius());
	slPlanetDensity.SetValue(p.GetDensity());
	slPlanetVelDirection.SetValue(p.GetVelDirection());
	slPlanetVelMagnitude.SetValue(p.GetVelMagnitude());
}

/**
	Update a slider in the menu.
	@param mouseX - the mouse x position.
	@param sys - the planet system object to trigger an event to.
*/
void Menu::UpdateSlider(float mouseX, PlanetSystem& sys)
{
	switch (currentSlider)
	{
	case MenuSlider::constG:
		slConstG.Update(mouseX);
		sys.SetGConst(slConstG.GetValue());
		break;
	case MenuSlider::pSize:
		slPlanetSize.Update(mouseX);
		sys.SetPlanetRadius(planetIndex, slPlanetSize.GetValue());
		break;
	case MenuSlider::pDensity:
		slPlanetDensity.Update(mouseX);
		sys.SetPlanetDensity(planetIndex, slPlanetDensity.GetValue());
		break;
	case MenuSlider::pVelDir:
		slPlanetVelDirection.Update(mouseX);
		sys.SetPlanetVelDir(planetIndex, slPlanetVelDirection.GetValue());
		break;
	case MenuSlider::pVelMag:
		slPlanetVelMagnitude.Update(mouseX);
		sys.SetPlanetVelMag(planetIndex, slPlanetVelMagnitude.GetValue());
		break;
	default:
		break;
	}
}

/**
	Update the information at the buttom of the menu.
	@param elapsed - the time passed since the last call.
	@param amount - the amount of planets currently in the simulation.
*/
void Menu::UpdateStats(Time elapsed, int amount)
{
	lifeTime += elapsed;
	std::string s = "Amount of planets: " + std::to_string(amount) + "\nYears passed : " + std::to_string(int(lifeTime.asSeconds()));
	stats.setString(s);
}

/**
	Check whether there is a need to track the mouse position.
	@return true if there is, or false otherwise.
*/
bool Menu::TrackMouse()
{
	return currentSlider != MenuSlider::none;
}

/**
	Check whether the menu is collapssing.
	@return true if it does, or false otherwise.
*/
bool Menu::MenuIsCollapsed()
{
	return (isCollapsed && menuBackground.getSize().x > 0) || (!isCollapsed && menuBackground.getSize().x < MENU_WIDTH);
}

/**
	Minimizing or maximizing the menu.
*/
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

/**
	Drawing every menu item.
*/
void Menu::draw(RenderTarget& target, RenderStates states) const
{
	states.texture = NULL;

	target.draw(menuBackground, states);
	target.draw(btnCollapseMenu, states);

	if (showMenuItems)
	{
		target.draw(menuTitle, states);
		target.draw(btnSwitchMenu, states);
		if (planetIndex == -1)
		{
			target.draw(slConstG, states);
			target.draw(cbVel, states);
			target.draw(cbAcc, states);
			target.draw(btnClear, states);
			target.draw(btnState, states);
			target.draw(stats, states);
		}
		else
		{
			target.draw(btnPreviousPlanet, states);
			target.draw(btnNextPlanet, states);
			target.draw(slPlanetSize, states);
			target.draw(slPlanetDensity, states);
			target.draw(slPlanetVelDirection, states);
			target.draw(slPlanetVelMagnitude, states);
			target.draw(btnPlanetDelete, states);
		}
	}
}
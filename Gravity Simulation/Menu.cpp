#include "Menu.h"

Menu::Menu(Font& font) :
	btnCollapseMenu(Vector2f(MENU_WIDTH + 6 + COLLAPSE_MENU_WIDTH / 2.0f + 5, COLLAPSE_MENU_WIDTH / 2.0f + 5), COLLAPSE_MENU_WIDTH, COLLAPSE_MENU_WIDTH, "<", ">", font, 64, Color(128, 128, 128, 200), Color(128, 128, 128, 200)),
	btnSwitchMenu(Vector2f(MENU_WIDTH / 2, 120), 300, 40, "Edit Planets", "Back to Main Menu", font, 32, Color::Cyan, Color::White),
	isCollapsed(false),
	showMenuItems(true),
	planetIndex(-1),
	currentSlider(none),
	slConstG(Vector2f(MENU_WIDTH / 2, 250), 0, 100, 50, font),
	cbVel(350, "Show Planet's Velocity", font, true),
	cbAcc(450, "Show Planet's Acceleration", font, false),
	btnState(Vector2f(MENU_WIDTH / 2, 550), 100, 50, "Start", "Pause", font, 32, Color::White, Color::Magenta),
	btnPreviousPlanet(Vector2f(MENU_WIDTH / 2 - 40, 200), 50, 35, "<-", "<-", font, 64, Color(204, 255, 255), Color(102, 102, 255)),
	btnNextPlanet(Vector2f(MENU_WIDTH / 2 + 40, 200), 50, 35, "->", "->", font, 64, Color(204, 255, 255), Color(102, 102, 255)),
	slPlanetSize(Vector2f(MENU_WIDTH / 2, 280), 10, 500, 10, font),
	slPlanetDensity(Vector2f(MENU_WIDTH / 2, 400), 0.5, 6, 1, font),
	slPlanetVelDirection(Vector2f(MENU_WIDTH / 2, 480), 0, 360, 0, font),
	slPlanetVelMagnitude(Vector2f(MENU_WIDTH / 2, 560), 0, 5, 0, font),
	btnPlanetDelete(Vector2f(MENU_WIDTH / 2, 650), 250, 50, "Remove Planet", "Remove Planet", font, 32, Color::White, Color::Red)
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
				currentSlider = constG;
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
			else if (btnState.contains(mousePos))
			{
				btnState.ButtonPressed();
				sys.SetState();
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
				currentSlider = pSize;
			}
			else if (slPlanetDensity.contains(mousePos))
			{
				slPlanetDensity.SetModify(true);
				currentSlider = pDensity;
			}
			else if (slPlanetVelDirection.contains(mousePos))
			{
				slPlanetVelDirection.SetModify(true);
				currentSlider = pVelDir;
			}
			else if (slPlanetVelMagnitude.contains(mousePos))
			{
				slPlanetVelMagnitude.SetModify(true);
				currentSlider = pVelMag;
			}
			else if (btnPlanetDelete.contains(mousePos))
			{
				sys.RemovePlanet(planetIndex);
				SwitchMenus(sys);
			}
		}
	}
}

void Menu::MouseReleased()
{
	switch (currentSlider)
	{
	case constG:
		slConstG.SetModify(false);
		break;
	case pSize:
		slPlanetSize.SetModify(false);
		break;
	case pDensity:
		slPlanetDensity.SetModify(false);
		break;
	case pVelDir:
		slPlanetVelDirection.SetModify(false);
		break;
	case pVelMag:
		slPlanetVelMagnitude.SetModify(false);
		break;
	default:
		break;
	}
	currentSlider = none;
}

void Menu::SwitchMenus(PlanetSystem& sys)
{
	btnSwitchMenu.ButtonPressed();
	if (planetIndex == -1)
	{
		planetIndex = 0;
		EditPlanet(sys.Editing(planetIndex));
	}
	else
	{
		planetIndex = -1;
		sys.RemoveOutlines();
	}
}

void Menu::EditPlanet(const Planet& p) // fill values of menu items with current planet
{
	slPlanetSize.SetValue(p.GetRadius());
	slPlanetDensity.SetValue(p.GetDensity());
	slPlanetVelDirection.SetValue(p.GetVelDirection());
	slPlanetVelMagnitude.SetValue(p.GetVelMagnitude());
}

void Menu::UpdateSlider(float mouseX, PlanetSystem& sys)
{
	switch (currentSlider)
	{
	case constG:
		slConstG.Update(mouseX);
		sys.SetGConst(slConstG.GetValue());
		break;
	case pSize:
		slPlanetSize.Update(mouseX);
		sys.SetPlanetRadius(planetIndex, slPlanetSize.GetValue());
		break;
	case pDensity:
		slPlanetDensity.Update(mouseX);
		sys.SetPlanetDensity(planetIndex, slPlanetDensity.GetValue());
		break;
	case pVelDir:
		slPlanetVelDirection.Update(mouseX);
		sys.SetPlanetVelDir(planetIndex, slPlanetVelDirection.GetValue());
		break;
	case pVelMag:
		slPlanetVelMagnitude.Update(mouseX);
		sys.SetPlanetVelMag(planetIndex, slPlanetVelMagnitude.GetValue());
		break;
	default:
		break;
	}
}

bool Menu::TrackMouse()
{
	return currentSlider != none;
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
		target.draw(btnSwitchMenu, states);
		if (planetIndex == -1)
		{
			target.draw(slConstG, states);
			target.draw(btnState, states);
			target.draw(cbVel, states);
			target.draw(cbAcc, states);
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
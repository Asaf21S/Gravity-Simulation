#include "Menu.h"

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
	cbMouseForce(460, "Use Mouse as Gravitational Force", font, false),
	btnSunEarth(Vector2f(MENU_WIDTH / 2, 540), 275, 50, "Sun-Earth", "Sun-Earth", font, 28, Color::White, Color(153, 255, 153)),
	btnAddFollower(Vector2f(MENU_WIDTH / 2, 615), 275, 50, "Add Follower", "Add Follower", font, 28, Color::White, Color::Cyan),
	btnClear(Vector2f(MENU_WIDTH / 2, 690), 275, 50, "Remove Everything", "Remove Everything", font, 28, Color::White, Color::Red),
	btnState(Vector2f(MENU_WIDTH / 2, 765), 275, 50, "Start", "Pause", font, 28, Color::White, Color::Magenta),
	btnPreviousPlanet(Vector2f(MENU_WIDTH / 2 - 40, 190), 50, 35, "<-", "<-", font, 64, Color(204, 255, 255), Color(102, 102, 255)),
	btnNextPlanet(Vector2f(MENU_WIDTH / 2 + 40, 190), 50, 35, "->", "->", font, 64, Color(204, 255, 255), Color(102, 102, 255)),
	btnPreviousSurface(Vector2f(MENU_WIDTH / 2 - 105, 300), 20, 55, "<", "<", font, 56, Color(192, 192, 192), Color(192, 192, 192)),
	planetDisplay(Vector2f(MENU_WIDTH / 2, 320), 0),
	btnNextSurface(Vector2f(MENU_WIDTH / 2 + 105, 300), 20, 55, ">", ">", font, 56, Color(192, 192, 192), Color(192, 192, 192)),
	slPlanetSize(Vector2f(MENU_WIDTH / 2, 450), 10, 500, 10, "Planet Size", font),
	slPlanetDensity(Vector2f(MENU_WIDTH / 2, 550), 0.5, 6, 1, "Planet Density", font),
	slPlanetVelDirection(Vector2f(MENU_WIDTH / 2, 650), 0, 360, 0, "Velocity Direction", font),
	slPlanetVelMagnitude(Vector2f(MENU_WIDTH / 2, 750), 0, 5, 0, "Velocity Magnitude", font),
	btnPlanetDelete(Vector2f(MENU_WIDTH / 2, 850), 250, 50, "Remove Planet", "Remove Planet", font, 28, Color::White, Color::Red)
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

	surfaceName.setFont(font);
	surfaceName.setString("");
	surfaceName.setCharacterSize(28);
	surfaceName.setOrigin(surfaceName.getLocalBounds().left + surfaceName.getLocalBounds().width / 2.0f, surfaceName.getLocalBounds().top + surfaceName.getLocalBounds().height / 2.0f);
	surfaceName.setPosition(MENU_WIDTH / 2, 240);
	
	if (!lockedTexture.loadFromFile("Textures/locked.png"))
		std::cerr << "Unable to load locked texture." << std::endl;
	if (!unlockedTexture.loadFromFile("Textures/unlocked.png"))
		std::cerr << "Unable to load unlocked texture." << std::endl;

	lockedSprite.setTexture(lockedTexture);
	unlockedSprite.setTexture(unlockedTexture);
	lockedSprite.setScale(0.15, 0.15);
	unlockedSprite.setScale(0.15, 0.15);
	lockedSprite.setPosition(MENU_WIDTH / 2 + 20, 335);
	unlockedSprite.setPosition(MENU_WIDTH / 2 + 20, 335);
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
	if (cbMouseForce.IsChecked()) sys.ToggleMouseForce();
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
			else if (cbMouseForce.contains(mousePos))
			{
				cbMouseForce.Clicked();
				sys.ToggleMouseForce();
			}
			else if (btnSunEarth.contains(mousePos))
			{
				btnSunEarth.ButtonPressed();
				sys.ReadyTemplate();
			}
			else if (btnAddFollower.contains(mousePos))
			{
				btnAddFollower.ButtonPressed();
				sys.CreateFollower(mousePos);
			}
			else if (btnClear.contains(mousePos))
			{
				btnClear.ButtonPressed();
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
			else if (btnPreviousSurface.contains(mousePos))
			{
				btnPreviousSurface.ButtonPressed();
				int newSurface = sys.ChangeSurface(planetIndex, false);
				UpdateSurfaceName(newSurface);
			}
			else if (lockedSprite.getGlobalBounds().contains(mousePos))
			{
				isLocked = !isLocked;
				sys.ToggleLock(planetIndex);
			}
			else if (btnNextSurface.contains(mousePos))
			{
				btnNextSurface.ButtonPressed();
				int newSurface = sys.ChangeSurface(planetIndex, true);
				UpdateSurfaceName(newSurface);
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
void Menu::SwitchMenus(PlanetSystem& sys, int pInd, std::pair<int, int> planetsExploded)
{
	if (planetIndex == -1) // currently on the main menu
	{
		if (planetsExploded.first == -1)
		{
			btnSwitchMenu.ButtonPressed();
			if (pInd == -1) planetIndex = 0; // button is clicked
			else planetIndex = pInd; // planet is clicked
			EditPlanet(sys.Editing(planetIndex));
		}
	}
	else // currently on the 'edit planet' menu
	{
		if (pInd == -1) // switch to the main menu
		{
			// don't switch to main menu if planets collide and we're not currently editting one of the the planets that has been exploded.
			if (planetsExploded.first == -1 || planetsExploded.first == planetIndex || planetsExploded.second == planetIndex)
			{
				btnSwitchMenu.ButtonPressed();
				planetIndex = -1;
				sys.RemoveOutlines();

				// if a planet has exploded and we currently modifying one of its slider, need to stop that.
				MouseReleased();
			}
			else if (planetsExploded.first != -1 && planetsExploded.first < planetIndex) planetIndex--;
			else if (planetsExploded.second != -1 && planetsExploded.second < planetIndex) planetIndex--;
		}
		else // edit a different planet
		{
			planetIndex = pInd; // planet is clicked
			EditPlanet(sys.Editing(planetIndex));
		}
	}
}

void Menu::UpdatePlanetDisplay(const Planet& p, int pInd)
{
	if (pInd == -1 || pInd == planetIndex)
	{
		planetDisplay = p;
		planetDisplay.LockPlanet(true, Vector2f(MENU_WIDTH / 2, 320));
	}
}

void Menu::UpdateVelocitySliders(int pInd, float dir, float mag)
{
	if (pInd == planetIndex)
	{
		slPlanetVelDirection.SetValue(dir);
		slPlanetVelMagnitude.SetValue(mag);
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
	UpdateSurfaceName(p.GetSurface());
	UpdatePlanetDisplay(p);
	isLocked = p.IsLocked();
}

void Menu::UpdateSurfaceName(int surfaceIndex)
{
	std::string s;
	switch (surfaceIndex)
	{
	case 0:
		s = "Earth";
		break;
	case 1:
		s = "Jupiter";
		break;
	case 2:
		s = "Mars";
		break;
	case 3:
		s = "Mercury";
		break;
	case 4:
		s = "Moon";
		break;
	case 5:
		s = "Neptune";
		break;
	case 6:
		s = "Saturn";
		break;
	case 7:
		s = "Sun";
		break;
	case 8:
		s = "Uranus";
		break;
	case 9:
		s = "Venus";
		break;
	default:
		s = "Error In surface's name";
		break;
	}
	surfaceName.setString(s);
	surfaceName.setOrigin(surfaceName.getLocalBounds().left + surfaceName.getLocalBounds().width / 2.0f, surfaceName.getLocalBounds().top + surfaceName.getLocalBounds().height / 2.0f);
	surfaceName.setPosition(MENU_WIDTH / 2, 240);
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
		UpdatePlanetDisplay(sys.SetPlanetRadius(planetIndex, slPlanetSize.GetValue()));
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
void Menu::UpdateStats(Time elapsed, bool isPaused, int amount)
{
	if (!isPaused) lifeTime += elapsed;
	std::string s = "Amount of planets: " + std::to_string(amount) + "\nYears passed : " + std::to_string(int(lifeTime.asSeconds()));
	stats.setString(s);

	btnSwitchMenu.Update(elapsed);
	btnSunEarth.Update(elapsed);
	btnAddFollower.Update(elapsed);
	btnClear.Update(elapsed);
	btnState.Update(elapsed);
	btnPlanetDelete.Update(elapsed);
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
			target.draw(cbMouseForce, states);
			target.draw(btnSunEarth, states);
			target.draw(btnAddFollower, states);
			target.draw(btnClear, states);
			target.draw(btnState, states);
			target.draw(stats, states);
		}
		else
		{
			target.draw(btnPreviousPlanet, states);
			target.draw(btnNextPlanet, states);
			target.draw(btnPreviousSurface, states);
			target.draw(surfaceName, states);
			target.draw(planetDisplay, states);
			if (isLocked)
				target.draw(lockedSprite, states);
			else
				target.draw(unlockedSprite, states);
			target.draw(btnNextSurface, states);
			target.draw(slPlanetSize, states);
			target.draw(slPlanetDensity, states);
			if (!isLocked)
			{
				target.draw(slPlanetVelDirection, states);
				target.draw(slPlanetVelMagnitude, states);
			}
			target.draw(btnPlanetDelete, states);
		}
	}
}
#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <functional>
#include "Slider.h"
#include "Button.h"
#include "CheckBox.h"
#include "PlanetSystem.h"
using namespace sf;

class Slider;
class Button;
class CheckBox;

const float MENU_WIDTH = 400.0f; // can't be less than 40
const float COLLAPSE_MENU_WIDTH = 60.0f;

enum class MenuSlider
{
	none, constG, pSize, pDensity, pVelDir, pVelMag
};

class Menu : public Drawable, public Transformable
{
	Time lifeTime;
	RectangleShape menuBackground;
	Button btnCollapseMenu;
	Text menuTitle;
	Button btnSwitchMenu;
	Font font;
	bool isCollapsed, showMenuItems;
	int planetIndex;
	MenuSlider currentSlider;

	// Main menu items
	Slider slConstG;
	CheckBox cbVel;
	CheckBox cbAcc;
	Button btnClear;
	Button btnState;
	Text stats;

	// Edit planet menu items
	Button btnPreviousPlanet;
	Button btnNextPlanet;
	Slider slPlanetSize;
	Slider slPlanetDensity;
	Slider slPlanetVelDirection;
	Slider slPlanetVelMagnitude;
	Button btnPlanetDelete;

	void EditPlanet(const Planet& p);
	virtual void draw(RenderTarget& target, RenderStates states) const;

public:
	Menu(Font& font);
	void Init(PlanetSystem& sys, int height);
	bool IsClickInside(Vector2f mousePos);
	void MouseClicked(Vector2f mousePos, PlanetSystem& sys);
	void MouseReleased();
	void SwitchMenus(PlanetSystem& sys, int pInd = -1);
	bool TrackMouse();
	bool MenuIsCollapsed();
	void CollapseMenu();
	void UpdateSlider(float mouseX, PlanetSystem& sys);
	void UpdateStats(Time elapsed, int amount);
};
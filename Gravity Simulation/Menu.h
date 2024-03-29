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

const float MENU_WIDTH = 450.0f; // can't be less than 40
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
	CheckBox cbMouseForce;
	Button btnSunEarth;
	Button btnAddFollower;
	Button btnClear;
	Button btnState;
	Text stats;

	// Edit planet menu items
	Button btnPreviousPlanet;
	Button btnNextPlanet;
	Button btnPreviousSurface;
	Text surfaceName;
	Planet planetDisplay;
	Texture lockedTexture, unlockedTexture;
	Sprite lockedSprite, unlockedSprite;
	bool isLocked;
	Button btnNextSurface;
	Slider slPlanetSize;
	Slider slPlanetDensity;
	Slider slPlanetVelDirection;
	Slider slPlanetVelMagnitude;
	Button btnPlanetDelete;

	void EditPlanet(const Planet& p);
	void UpdateSurfaceName(int surfaceIndex);
	virtual void draw(RenderTarget& target, RenderStates states) const;

public:
	Menu(Font& font);
	void Init(PlanetSystem& sys, int height);
	bool IsClickInside(Vector2f mousePos);
	void MouseClicked(Vector2f mousePos, PlanetSystem& sys);
	void MouseReleased();
	void SwitchMenus(PlanetSystem& sys, int pInd = -1, std::pair<int, int> planetsExploded = std::pair<int, int>(-1, -1));
	void UpdatePlanetDisplay(const Planet& p, int pInd = -1);
	void UpdateVelocitySliders(int pInd, float dir, float mag);
	bool TrackMouse();
	bool MenuIsCollapsed();
	void CollapseMenu();
	void UpdateSlider(float mouseX, PlanetSystem& sys);
	void UpdateStats(Time elapsed, bool isPaused, int amount);
};
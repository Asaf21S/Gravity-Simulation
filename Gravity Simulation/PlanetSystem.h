#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Planet.h"
#include "Menu.h"
using namespace sf;

class Menu;

class PlanetSystem : public Drawable, public Transformable
{
    std::vector<Planet> planets;
    bool isPaused;
    float GravitationalConst;
    bool showVel, showAcc;
    bool showTrail;
    bool expandPlanet;
    int setVelocityInd;
    float currentMaxR;

    void CheckIndex(int index);
    virtual void draw(RenderTarget& target, RenderStates states) const;

public:
    PlanetSystem();
    void MouseClicked(Vector2f mousePos, Menu& menu);
    int GetAmount();
    void Expand(int index = -1);
    void Update(Time elapsed);
    void RemovePlanet(int index = -1);
    void ClearEverything();
    bool GetState(); // maybe we dont even need it
    void ToggleState();
    void ToggleArrowVisibility(bool isVel);
    bool TrackMouse();
    void UpdateArrow(Vector2f mousePos);
    void StopExpanding(bool isRemoved, int index = -1);
    void SetGConst(float value);
    const Planet& Editing(int index);
    void RemoveOutlines();
    void SetPlanetRadius(int index, float radius);
    void SetPlanetDensity(int index, float density);
    void SetPlanetVelDir(int index, float dir);
    void SetPlanetVelMag(int index, float mag);
};
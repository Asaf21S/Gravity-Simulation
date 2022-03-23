#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Planet.h"
using namespace sf;

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

    virtual void draw(RenderTarget& target, RenderStates states) const;

public:
    PlanetSystem();
    void MouseClicked(Vector2f mousePos);
    void Expand(int index = -1);
    void Update(Time elapsed);
    static float Dist(Vector2f p1, Vector2f p2);
    void RemovePlanet(int index = -1);
    bool GetState(); // maybe we dont even need it
    void SetState();
    void SetVelVisibility();
    void SetAccVisibility();
    bool TrackMouse();
    void UpdateArrow(Vector2f mousePos);
    void StopExpanding(bool isRemoved, int index = -1);
    void SetGConst(float value);
};
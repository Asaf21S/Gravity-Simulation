#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Planet.h"
using namespace sf;
const float GRAVITATIONAL_CONSTANT = 0.2e-7;

class PlanetSystem : public Drawable, public Transformable
{
    std::vector<Planet> planets;
    bool showVelocity;
    bool showAcceleration;
    bool showTrail;

    virtual void draw(RenderTarget& target, RenderStates states) const;

public:
    PlanetSystem();
    void AddPlanet(Planet planet);
    void Expand();
    void Update(Time elapsed);

    bool pause;
};
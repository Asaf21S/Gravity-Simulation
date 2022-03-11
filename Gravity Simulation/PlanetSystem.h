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
    bool expandPlanet;
    float currentMaxR;

    virtual void draw(RenderTarget& target, RenderStates states) const;

public:
    PlanetSystem();
    void AddPlanet(Planet planet);
    void Expand(int index = -1);
    void Update(Time elapsed);
    float Dist(Vector2f p1, Vector2f p2);
    void RemovePlanet(int index = -1);
    void StopExpanding();

    bool pause;
};
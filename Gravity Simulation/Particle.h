#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;

class Particle : public Drawable, public Transformable
{
    friend class PlanetSystem;

    CircleShape particle;
    Vector2f velocity;
    Color color;
    Time lifeTime;
    float fadingTime;

    void Update(Time elapsed);
    virtual void draw(RenderTarget& target, RenderStates states) const;

public:
    Particle(Vector2f position, float radius, Vector2f velocity, int surfaceIndex);
    bool IsVisible();
};
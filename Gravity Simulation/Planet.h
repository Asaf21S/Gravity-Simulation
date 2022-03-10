#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;

class Planet : public Drawable, public Transformable
{
    CircleShape planet;
    float density;
    float mass;
    Vector2f velocity;
    Vector2f acceleration;
    Time lifeTime;

    void UpdateMass();
    virtual void draw(RenderTarget& target, RenderStates states) const;

public:
    Planet(Vector2f position, float radius);
    void SetVelocity(Vector2f vel);
    void Expand();
    Vector2f GetPosition();
    float GetMass();
    void AddForce(Vector2f force);
    void Update(Time elapsed, bool isPaused);
};
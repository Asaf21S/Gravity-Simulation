#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;

const float ARROW_LENGTH_TO_VELOCITY = 140;
const float ARROW_OFFSET = 5;

class Planet : public Drawable, public Transformable
{
    friend class PlanetSystem;

    CircleShape planet;
    float density;
    float mass;
    Vector2f velocity;
    Vector2f acceleration;
    VertexArray velArrow;
    bool showVelArrow;
    Time lifeTime;

    void SetArrow(Vector2f mouse);
    void UpdateArrow();
    void SetVelocity(Vector2f vel);
    void Expand();
    Vector2f GetPosition();
    float GetRadius();
    float GetMass();
    void AddForce(Vector2f force);
    void Update(Time elapsed, bool isPaused, bool showVelArrow);
    void UpdateMass();
    virtual void draw(RenderTarget& target, RenderStates states) const;

public:
    Planet(Vector2f position);
};
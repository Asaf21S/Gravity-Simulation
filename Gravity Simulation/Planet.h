#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;

const float ARROW_LENGTH_TO_VELOCITY = 75;
const float ARROW_LENGTH_TO_ACCELERATION = 1000;
const float ARROW_OFFSET = 5;
const int TEXTURES_AMOUNT = 10;

class Planet : public Drawable, public Transformable
{
    friend class PlanetSystem;

    CircleShape planet;
    float density; // in g/cm^3    { https://www.google.com/search?q=density+of+a+planet&oq=density+of+a+planet&aqs=chrome..69i57.7577j0j7&sourceid=chrome&ie=UTF-8 }
    float mass;
    Vector2f velocity;
    VertexArray velArrow;
    bool showVelArrow;
    bool velTooSmall;
    Vector2f velDirection;
    Vector2f acceleration;
    VertexArray accArrow;
    bool showAccArrow;
    bool accTooSmall;
    int planetSurfaceInd;
    float spinAngle, spinSpeed;
    bool isLocked;
    Time lifeTime;

    void SetArrow(Vector2f arrowPoint, bool isVel);
    void UpdateArrow(bool isVel);
    void SetVelocity(Vector2f vel);
    void ToggleArrowVisibility(bool isVel);
    void Expand();
    void SetPosition(Vector2f position);
    Vector2f GetPosition();
    void SetTexture(Texture& tex);
    float GetMass();
    void AddForce(Vector2f force);
    void Update(Time elapsed, bool isPaused);
    void UpdateMass();
    virtual void draw(RenderTarget& target, RenderStates states) const;

public:
    Planet(Vector2f position, int surfaceIndex);
    void LockPlanet(bool display, Vector2f position = Vector2f(0.0f, 0.0f));
    void SetRadius(float radius);
    float GetRadius() const;
    float GetDensity() const;
    float GetVelDirection() const;
    float GetVelMagnitude() const;
    int GetSurface() const;
    bool IsLocked() const;
    static float Dist(Vector2f p1, Vector2f p2 = Vector2f(0.0f, 0.0f));
};
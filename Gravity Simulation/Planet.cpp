#include "Planet.h"

Planet::Planet(Vector2f position, float radius) :
    planet(radius),
    density(1.f),
    acceleration(0, 0)
{
    planet.setPosition(position);
    UpdateMass();
}

void Planet::SetVelocity(Vector2f vel)
{
    velocity = vel;
}

void Planet::Expand()
{
    float factor = 1;
    planet.setRadius(planet.getRadius() + factor);
    planet.move(-factor, -factor);
    UpdateMass();
}

Vector2f Planet::GetPosition()
{
    return Vector2f(planet.getPosition().x + planet.getRadius(), planet.getPosition().y + planet.getRadius());
}

float Planet::GetMass()
{
    return mass;
}

void Planet::AddForce(Vector2f force)
{
    acceleration += force;
}

void Planet::Update(sf::Time elapsed, bool isPaused)
{
    if (!isPaused)
    {
        lifeTime += elapsed;
        planet.move(velocity);
        velocity += acceleration;
    }

    acceleration.x = 0;
    acceleration.y = 0;
}

void Planet::UpdateMass()
{
    // mass = density * volume
    // volume = (4/3)πr^3
    mass = (density * 4.0 * M_PI * pow(planet.getRadius(), 3)) / 3.0;
}

void Planet::draw(RenderTarget& target, RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // our planet don't use a texture
    states.texture = NULL;

    // draw the planet
    target.draw(planet, states);
}
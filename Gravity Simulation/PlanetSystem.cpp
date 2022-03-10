#include "PlanetSystem.h"

PlanetSystem::PlanetSystem() :
    pause(true),
    showVelocity(false),
    showAcceleration(false),
    showTrail(false)
{}

void PlanetSystem::AddPlanet(Planet planet)
{
    planets.push_back(planet);
}

void PlanetSystem::Expand()
{
    planets.back().Expand();
}

void PlanetSystem::Update(sf::Time elapsed)
{
    float len, mag;
    for (int i = 0; i < planets.size(); i++)
    {
        for (int j = 0; j < planets.size(); j++)
        {
            if (i != j)
            {
                Vector2f force = planets[j].GetPosition() - planets[i].GetPosition();
                len = hypot(force.x, force.y);
                force.x /= len;
                force.y /= len;
                mag = GRAVITATIONAL_CONSTANT * planets[i].GetMass() * planets[j].GetMass() / pow(len, 2);
                force.x *= mag;
                force.y *= mag;
                planets[i].AddForce(force);
            }
        }
        
        planets[i].Update(elapsed, pause);
    }
}

void PlanetSystem::draw(RenderTarget& target, RenderStates states) const
{
    states.texture = NULL;

    // draw the planets
    for (int i = 0; i < planets.size(); i++)
    {
        target.draw(planets[i], states);
    }
}
#include "PlanetSystem.h"

PlanetSystem::PlanetSystem() :
    pause(true),
    showVelocity(false),
    showAcceleration(false),
    showTrail(false),
    expandPlanet(false),
    currentMaxR(500)
{}

void PlanetSystem::AddPlanet(Planet planet)
{
    float dist;
    currentMaxR = 500;
    for (int i = 0; i < planets.size(); i++)
    {
        dist = Dist(planets[i].GetPosition(), planet.GetPosition()) - planets[i].GetRadius();
        if (dist < currentMaxR) currentMaxR = dist;
    }
    planets.push_back(planet);
    if (planets[planets.size() - 1].GetRadius() >= currentMaxR) RemovePlanet();
    else expandPlanet = true;
}

void PlanetSystem::Expand(int index)
{
    if (index == -1) index += planets.size();
    planets[index].Expand();
    if (planets[index].GetRadius() >= currentMaxR) RemovePlanet(index);
}

void PlanetSystem::Update(sf::Time elapsed)
{
    if (expandPlanet) Expand();
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

float PlanetSystem::Dist(Vector2f p1, Vector2f p2)
{
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

void PlanetSystem::RemovePlanet(int index)
{
    if (index == -1) index += planets.size();
    planets.erase(planets.begin() + index);
    StopExpanding();
}

void PlanetSystem::StopExpanding()
{
    expandPlanet = false;
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
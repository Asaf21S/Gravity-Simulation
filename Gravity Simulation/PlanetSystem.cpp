#include "PlanetSystem.h"

PlanetSystem::PlanetSystem() :
    GravitationalConst(1e-2),
    isPaused(true),
    showTrail(false),
    expandPlanet(false),
    setVelocityInd(-1),
    currentMaxR(500)
{}

void PlanetSystem::MouseClicked(Vector2f mousePos)
{
    if (isPaused)
    {
        if (setVelocityInd != -1) setVelocityInd = -1;
        else
        {
            planets.push_back(Planet(mousePos));
            float dist;
            currentMaxR = 500;
            for (int i = 0; i < planets.size() - 1; i++)
            {
                dist = Dist(planets[i].GetPosition(), mousePos) - planets[i].GetRadius();
                if (dist < currentMaxR) currentMaxR = dist;
            }
            if (planets.back().GetRadius() >= currentMaxR) RemovePlanet();
            else expandPlanet = true;
        }
    }
}

void PlanetSystem::Expand(int index)
{
    if (index == -1) index += planets.size();
    planets[index].Expand();
    if (planets[index].GetRadius() >= currentMaxR) RemovePlanet(index);
}

void PlanetSystem::Update(Time elapsed)
{
    if (expandPlanet) Expand();
    float len, mag;
    for (int i = 0; i < planets.size(); i++)
    {
        planets[i].acceleration.x = 0;
        planets[i].acceleration.y = 0;
        for (int j = 0; j < planets.size(); j++)
        {
            if (i != j)
            {
                Vector2f force = planets[j].GetPosition() - planets[i].GetPosition();
                len = hypot(force.x, force.y);
                force.x /= len;
                force.y /= len;
                mag = GravitationalConst * planets[i].GetMass() * planets[j].GetMass() / pow(len, 2);
                force.x *= mag;
                force.y *= mag;
                planets[i].AddForce(force);
            }
        }
        // need to call planets[i].SetArrowVisibility before calling planets[i].Update
        planets[i].Update(elapsed, isPaused);
    }
}

void PlanetSystem::UpdateArrow(Vector2f mousePos)
{
    planets[setVelocityInd].SetArrow(mousePos, true);
}

bool PlanetSystem::TrackMouse()
{
    return setVelocityInd != -1;
}

float PlanetSystem::Dist(Vector2f p1, Vector2f p2)
{
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

void PlanetSystem::RemovePlanet(int index)
{
    if (index == -1) index += planets.size();
    planets.erase(planets.begin() + index);
    StopExpanding(true);
}

bool PlanetSystem::GetState()
{
    return isPaused;
}

void PlanetSystem::SetState()
{
    isPaused = !isPaused;
}

void PlanetSystem::SetVelVisibility()
{
    showVel = !showVel;
    for (int i = 0; i < planets.size(); i++)
    {
        planets[i].SetArrowVisibility(true);
    }
}

void PlanetSystem::SetAccVisibility()
{
    showAcc = !showAcc;
    for (int i = 0; i < planets.size(); i++)
    {
        planets[i].SetArrowVisibility(false);
    }
}

void PlanetSystem::StopExpanding(bool isRemoved, int index)
{
    if (expandPlanet)
    {
        expandPlanet = false;
        if (!isRemoved)
        {
            setVelocityInd = (index + planets.size()) % planets.size();
            if (showVel) planets[setVelocityInd].SetArrowVisibility(true);
            if (showAcc) planets[setVelocityInd].SetArrowVisibility(false);
        }
    }
}

void PlanetSystem::SetGConst(float value)
{
    GravitationalConst = value / 1e4;
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
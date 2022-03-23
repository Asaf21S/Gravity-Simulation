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
                dist = Planet::Dist(planets[i].GetPosition(), mousePos) - planets[i].GetRadius();
                if (dist < currentMaxR) currentMaxR = dist;
            }
            if (planets.back().GetRadius() >= currentMaxR) RemovePlanet();
            else
            {
                expandPlanet = true;
                if (showAcc) planets.back().SetArrowVisibility(false);
            }
        }
    }
}

int PlanetSystem::GetAmount()
{
    return planets.size();
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
        }
    }
}

void PlanetSystem::SetGConst(float value)
{
    GravitationalConst = value / 1e4;
}

const Planet& PlanetSystem::Editing(int index)
{
    CheckIndex(index);
    for (int i = 0; i < planets.size(); i++)
    {
        planets[i].planet.setOutlineThickness(0);
        if (i == index) planets[i].planet.setOutlineThickness(5);
    }
    return planets[index];
}

void PlanetSystem::RemoveOutlines()
{
    for (int i = 0; i < planets.size(); i++)
    {
        planets[i].planet.setOutlineThickness(0);
    }
}

void PlanetSystem::SetPlanetRadius(int index, float radius)
{
    CheckIndex(index);
    planets[index].SetRadius(radius);
}

void PlanetSystem::SetPlanetDensity(int index, float density)
{
    CheckIndex(index);
    planets[index].density = density;
}

void PlanetSystem::SetPlanetVelDir(int index, float dir)
{
    CheckIndex(index);
    float mag = planets[index].GetVelMagnitude();
    dir = 360 - dir;
    dir *= M_PI / 180.0f;
    planets[index].SetVelocity(Vector2f(mag * cos(dir), mag * sin(dir)));
}

void PlanetSystem::SetPlanetVelMag(int index, float mag)
{
    CheckIndex(index);
    if (mag == 0) mag = 0.001;
    planets[index].velocity *= mag / planets[index].GetVelMagnitude();
}

void PlanetSystem::CheckIndex(int index)
{
    if (index >= planets.size() || index < 0)
    {
        std::string msg = "Error: can't find the planet. Requested index: " + std::to_string(index) + ".";
        throw std::range_error(msg);
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
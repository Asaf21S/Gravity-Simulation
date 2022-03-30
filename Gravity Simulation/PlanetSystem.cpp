#include "PlanetSystem.h"

/**
    PlanetSystem constructor.
*/
PlanetSystem::PlanetSystem() :
    GravitationalConst(1e-2),
    showVel(false),
    showAcc(false),
    isPaused(true),
    showTrail(false),
    expandPlanet(false),
    setVelocityInd(-1),
    currentMaxR(500)
{
    srand(time(0));
}

/**
    Called when the mouse is clicked on the simulation.
    @param mousePos - the position of the mouse.
    @param menu - the menu object that will display the edit menu if the click was inside a planet.
*/
void PlanetSystem::MouseClicked(Vector2f mousePos, Menu& menu)
{
    if (setVelocityInd != -1)
    {
        if (!showVel) planets[setVelocityInd].ToggleArrowVisibility(true);
        setVelocityInd = -1;
    }
    else
    {
        bool inPlanet = false;
        for (int i = 0; i < planets.size(); i++)
        {
            if (Planet::Dist(planets[i].GetPosition(), mousePos) <= planets[i].GetRadius())
            {
                menu.SwitchMenus(*this, i);
                inPlanet = true;
                break;
            }
        }
        if (isPaused && !inPlanet)
        {
            // adding new planet
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
                if (showAcc) planets.back().ToggleArrowVisibility(false);
            }

        }
    }
}

/**
    Get the amount of planets.
*/
int PlanetSystem::GetAmount()
{
    return planets.size();
}

/**
    Update the simulation on a new frame. 
    The function computes the gravitational forces between the planets, and check for collisions.
    @param elapsed - the time passed since the last call.
*/
void PlanetSystem::Update(Time elapsed)
{
    if (expandPlanet) Expand();
    float len, mag;
    std::vector<Planet> newPlanets;
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
                force /= len;
                mag = GravitationalConst * planets[i].GetMass() * planets[j].GetMass() / pow(len, 2);
                force *= mag;
                planets[i].AddForce(force);
            }
        }
        // need to call planets[i].SetArrowVisibility before calling planets[i].Update
        planets[i].Update(elapsed, isPaused);

        // check for collisions:
        for (int j = 0; j < i; j++)
        {
            Vector2f pos1 = planets[i].GetPosition(), pos2 = planets[j].GetPosition();
            float r1 = planets[i].GetRadius(), r2 = planets[j].GetRadius();
            if (Planet::Dist(pos1, pos2) <= r1 + r2)
            {
                if (r1 > 40)
                {
                    int amount = 3 + (rand() % 3);
                    for (int p = 0; p < amount; p++)
                    {
                        float phi = rand() % 360;
                        phi *= M_PI / 180;
                        Planet pl(pos1 + Vector2f(cos(phi), sin(phi)) * float(20 + (rand() % int(r1))));
                        pl.SetRadius(10 + (rand() % int(r1 / 2 - 9)));
                        Vector2f plVel = pl.GetPosition() - pos1;
                        plVel /= Planet::Dist(plVel);
                        pl.SetVelocity(plVel / float(30 + rand() % 5));
                        newPlanets.push_back(pl);
                    }
                }
                if (r2 > 40)
                {
                    int amount = 3 + (rand() % 3);
                    for (int p = 0; p < amount; p++)
                    {
                        float phi = rand() % 360;
                        phi *= M_PI / 180;
                        Planet pl(pos2 + Vector2f(cos(phi), sin(phi)) * float(20 + (rand() % int(r2))));
                        pl.SetRadius(10 + (rand() % int(r2 / 2 - 9)));
                        Vector2f plVel = pl.GetPosition() - pos2;
                        plVel /= Planet::Dist(plVel);
                        pl.SetVelocity(plVel / float(5 + rand() % 5));
                        newPlanets.push_back(pl);
                    }
                }
                planets.erase(planets.begin() + i);
                planets.erase(planets.begin() + j);
                break;
            }
        }
    }
    if (newPlanets.size() > 0)
    {
        // show the planets arrows if needed
        for(int i = 0; i < newPlanets.size(); i++)
        {
            if (showAcc) newPlanets[i].ToggleArrowVisibility(false);
            if (showVel) newPlanets[i].ToggleArrowVisibility(true);
        }
        
        // add new planets to the system
        planets.insert(planets.end(), newPlanets.begin(), newPlanets.end());
    }
}

/**
    Update the velocity arrow to point the mouse position.
    @param mousePos - the position of the mouse.
*/
void PlanetSystem::UpdateArrow(Vector2f mousePos)
{
    if (Planet::Dist(mousePos, planets[setVelocityInd].GetPosition()) < planets[setVelocityInd].GetRadius())
        planets[setVelocityInd].SetVelocity(Vector2f(0.0f, 0.0f));
    else
        planets[setVelocityInd].SetArrow(mousePos, true);
}

/**
    Check whether there is a need to track the mouse position.
    @return true if there is, or false otherwise.
*/
bool PlanetSystem::TrackMouse()
{
    return setVelocityInd != -1;
}

/**
    Remove a specific planet from the simulation.
    @param index - the index of the planet to be removed.
*/
void PlanetSystem::RemovePlanet(int index)
{
    if (index == -1) index += planets.size();
    planets.erase(planets.begin() + index);
    StopExpanding(true);
}

/**
    Remove every planet from the simulation.
*/
void PlanetSystem::ClearEverything()
{
    for (int i = planets.size() - 1; i >= 0; i--)
    {
        planets.erase(planets.begin() + i);
    }
    setVelocityInd = -1;
}

/**
    Toggle the state of the simulation - whether it's on pause or running.
*/
void PlanetSystem::ToggleState()
{
    isPaused = !isPaused;
}

/**
    Get the current state of the simulation
    @return true if it's paused, or false if it's running.
*/
bool PlanetSystem::GetState()
{
    return isPaused;
}

/**
    Toggle the velocity or acceleration arrow visibility for every planet.
    @param isVel - for velocity (true), for acceleration (false).
*/
void PlanetSystem::ToggleArrowVisibility(bool isVel)
{
    if (isVel) showVel = !showVel;
    else showAcc = !showAcc;
    for (int i = 0; i < planets.size(); i++)
    {
        planets[i].ToggleArrowVisibility(isVel);
    }
}

/**
    Increase the radius of a specific planet.
    @param index - the index of the planet to be expanded.
*/
void PlanetSystem::Expand(int index)
{
    if (index == -1) index += planets.size();
    planets[index].Expand();
    if (planets[index].GetRadius() >= currentMaxR) RemovePlanet(index);
}

/**
    @param isRemoved - whether to stop because the planet is removed or not.
    @param index - the index of the planet that should stop expanding.
*/
void PlanetSystem::StopExpanding(bool isRemoved, int index)
{
    if (expandPlanet)
    {
        expandPlanet = false;
        if (!isRemoved)
        {
            setVelocityInd = (index + planets.size()) % planets.size();
            planets[setVelocityInd].ToggleArrowVisibility(true);
        }
    }
}

/**
    Set the gravitational constant.
*/
void PlanetSystem::SetGConst(float value)
{
    GravitationalConst = value / 1e5;
}

/**
    Update the outline of a specific planet.
    @param index - the index of the planet that is being edited.
    @return the planet that is being edited.
*/
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

/**
    Remove the outline from every planet.
*/
void PlanetSystem::RemoveOutlines()
{
    for (int i = 0; i < planets.size(); i++)
    {
        planets[i].planet.setOutlineThickness(0);
    }
}

/**
    Set the radius of a specific planet.
    @param index - the index of the planet to change the radius to.
    @param radius - the value which the radius will be set to.
*/
void PlanetSystem::SetPlanetRadius(int index, float radius)
{
    CheckIndex(index);
    planets[index].SetRadius(radius);
}

/**
    Set the density of a specific planet.
    @param index - the index of the planet to change the density to.
    @param density - the value which the density will be set to.
*/
void PlanetSystem::SetPlanetDensity(int index, float density)
{
    CheckIndex(index);
    planets[index].density = density;
}

/**
    Set the velocity direction of a specific planet.
    @param index - the index of the planet to change the velocity direction to.
    @param dir - the direction which the velocity will be set to in degrees.
*/
void PlanetSystem::SetPlanetVelDir(int index, float dir)
{
    CheckIndex(index);
    float mag = planets[index].GetVelMagnitude();
    dir = 360 - dir;
    dir *= M_PI / 180.0f;
    planets[index].SetVelocity(Vector2f(mag * cos(dir), mag * sin(dir)));
}

/**
    Set the velocity magnitude of a specific planet.
    @param index - the index of the planet to change the velocity magnitude to.
    @param mag - the magnitude which the velocity will be set to.
*/
void PlanetSystem::SetPlanetVelMag(int index, float mag)
{
    CheckIndex(index);
    if (mag == 0) mag = 0.001;
    planets[index].velocity *= mag / planets[index].GetVelMagnitude();
}

/**
    Helper function that check whether an index is valid. If invalid, the function throws range_error exception.
    @param index - the index to check for.
*/
void PlanetSystem::CheckIndex(int index)
{
    if (index >= planets.size() || index < 0)
    {
        std::string msg = "Error: can't find the planet. Requested index: " + std::to_string(index) + ".";
        throw std::range_error(msg);
    }
}

/**
    Drawing every planet.
*/
void PlanetSystem::draw(RenderTarget& target, RenderStates states) const
{
    states.texture = NULL;

    // draw the planets
    for (int i = 0; i < planets.size(); i++)
    {
        target.draw(planets[i], states);
    }
}
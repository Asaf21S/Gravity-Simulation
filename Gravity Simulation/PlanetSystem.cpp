#include "PlanetSystem.h"

/**
    PlanetSystem constructor.
    @param windowSize - the size of the window.
*/
PlanetSystem::PlanetSystem(Vector2u windowSize) :
    windowSize(windowSize),
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

    int particleAmount = 100 + (rand() % 21);
    for (int p = 0; p < particleAmount; p++)
    {
        Vector2f pos = Vector2f(rand() % windowSize.x, rand() % windowSize.y);
        float radius = 1 + (rand() % 4);
        float degree = rand() % 360;
        degree *= M_PI / 180.0f;
        float mag = float(rand() % 80) / 1000.0;
        Vector2f vel = Vector2f(mag * cos(degree), mag * sin(degree));
        backgroundParticles.push_back(Particle(pos, radius, vel));
    }

    if (!planetShadowTexture.loadFromFile("Textures/planet.jpg"))
        std::cerr << "Unable to load texture 1." << std::endl;
    if (!planetTexture.loadFromFile("Textures/mercury.jpg"))
        std::cerr << "Unable to load texture 2." << std::endl; 

    planetShadowSprite.setTexture(planetShadowTexture);
    planetSprite.setTexture(planetTexture);
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
            planets.push_back(Planet(mousePos, finalTex));
            xValues.push_back(0.0f);
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
    // update particles
    if (!isPaused)
    {
        // delete background particles that outside the frame
        for (int i = 0; i < backgroundParticles.size(); i++)
        {
            backgroundParticles[i].Update();
            if (backgroundParticles[i].isSparkling) backgroundParticles[i].Sparkle(elapsed);
            if (backgroundParticles[i].particle.getPosition().x < -backgroundParticles[i].particle.getRadius() ||
                backgroundParticles[i].particle.getPosition().x > windowSize.x + backgroundParticles[i].particle.getRadius() ||
                backgroundParticles[i].particle.getPosition().y < -backgroundParticles[i].particle.getRadius() ||
                backgroundParticles[i].particle.getPosition().y > windowSize.y + backgroundParticles[i].particle.getRadius())
            {
                backgroundParticles.erase(backgroundParticles.begin() + i);
                break;
            }
        }
        // sparkle-effect
        if (rand() % 40 == 0)
        {
            // choose random particle:
            int i = rand() % backgroundParticles.size();
            backgroundParticles[i].isSparkling = true;
        }

        // delete explosion particles that outside the frame
        for (int i = 0; i < explosionParticles.size(); i++)
        {
            explosionParticles[i].Update();
            if (explosionParticles[i].particle.getPosition().x < -explosionParticles[i].particle.getRadius() ||
                explosionParticles[i].particle.getPosition().x > windowSize.x + explosionParticles[i].particle.getRadius() ||
                explosionParticles[i].particle.getPosition().y < -explosionParticles[i].particle.getRadius() ||
                explosionParticles[i].particle.getPosition().y > windowSize.y + explosionParticles[i].particle.getRadius())
            {
                explosionParticles.erase(explosionParticles.begin() + i);
                break;
            }
        }

        // rotate planets' texture
        for (int i = 0; i < planets.size(); i++)
        {
            xValues[i] += 0.5;
            if (xValues[i] >= planetTexture.getSize().x / 1.5) xValues[i] -= planetTexture.getSize().x / 1.5;
        }
    }

    // update planets
    if (expandPlanet) Expand();
    float len, mag;
    std::vector<Planet> newPlanets;
    for (int i = 0; i < planets.size(); i++)
    {
        // compute gravitational force
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

        // set the planet texture
        float r1 = planets[i].GetRadius();

        planetSprite.setTextureRect(IntRect(xValues[i], planetTexture.getSize().y / 2 - r1, 2 * r1, 2 * r1));
        planetShadowSprite.setScale(2 * r1 / planetShadowTexture.getSize().x, 2 * r1 / planetShadowTexture.getSize().y);
        RenderTexture renderTexture;
        renderTexture.create(2 * r1, 2 * r1);
        renderTexture.draw(planetSprite);
        renderTexture.draw(planetShadowSprite, sf::RenderStates(sf::BlendMultiply));
        renderTexture.display();
        finalTex = renderTexture.getTexture();        

        planets[i].Update(elapsed, isPaused);

        // check for collisions:
        Vector2f pos1 = planets[i].GetPosition();

        for (int j = 0; j < i; j++)
        {
            Vector2f pos2 = planets[j].GetPosition();
            float r2 = planets[j].GetRadius();

            if (Planet::Dist(pos1, pos2) <= r1 + r2) // planets collide
            {
                // add explosion particles
                Vector2f normal = Vector2f(pos1.y - pos2.y, pos2.x - pos1.x);
                Vector2f particlePosition = pos1 * r2 + pos2 * r1;
                particlePosition /= r1 + r2;
                float minVelMag = planets[i].GetVelMagnitude() > planets[j].GetVelMagnitude() ? planets[i].GetVelMagnitude() : planets[j].GetVelMagnitude();
                int maxVelMag = 3 * minVelMag > 10 ? 3 * minVelMag : 10;

                int particleAmount = 5 + (rand() % 6);
                for (int p = 0; p < particleAmount; p++) // add explosion particles to one side
                {
                    float radius = 3 + (rand() % 3);
                    float degree = atan2(normal.y, normal.x) * 180 / M_PI;
                    degree += (rand() % 21) - 10;
                    degree *= M_PI / 180.0f;
                    float mag = minVelMag + (rand() % maxVelMag);
                    Vector2f vel = Vector2f(mag * cos(degree), mag * sin(degree));
                    explosionParticles.push_back(Particle(particlePosition, radius, vel));
                }

                particleAmount = 5 + (rand() % 6);
                normal = -normal;
                for (int p = 0; p < particleAmount; p++) // add explosion particles to the second side
                {
                    float radius = 3 + (rand() % 3);
                    float degree = atan2(normal.y, normal.x) * 180 / M_PI;
                    degree += (rand() % 21) - 10;
                    degree *= M_PI / 180.0f;
                    float mag = minVelMag + (rand() % maxVelMag);
                    Vector2f vel = Vector2f(mag * cos(degree), mag * sin(degree));
                    explosionParticles.push_back(Particle(particlePosition, radius, vel));
                }

                if (r1 > 40 && r2 > 40) // both planets big enough
                {
                    bool planetOverriding;
                    float phi;
                    Planet pl(Vector2f(0.0f, 0.0f), finalTex);
                    Vector2f plVel(0.0f, 0.0f);

                    int amount = 3 + (rand() % 3);
                    for (int p = 0; p < amount; p++) // planet i explosion
                    {
                        do
                        {
                            planetOverriding = false;
                            phi = rand() % 360;
                            phi *= M_PI / 180;
                            pl.planet.setPosition(pos1 + Vector2f(cos(phi), sin(phi)) * float(20 + (rand() % int(r1))));
                            pl.SetRadius(10 + (rand() % int(r1 / 2 - 9)));

                            for (int newP = 0; newP < newPlanets.size(); newP++)
                            {
                                if (Planet::Dist(newPlanets[newP].GetPosition(), pl.GetPosition()) <= newPlanets[newP].GetRadius() + pl.GetRadius())
                                    planetOverriding = true;
                            }
                        } while (planetOverriding);
                        
                        plVel = pl.GetPosition() - pos1;
                        plVel /= Planet::Dist(plVel);
                        pl.SetVelocity(plVel / float(30 + rand() % 5));
                        newPlanets.push_back(pl);
                    }

                    amount = 3 + (rand() % 3);
                    for (int p = 0; p < amount; p++) // planet j explosion
                    {
                        do
                        {
                            planetOverriding = false;
                            phi = rand() % 360;
                            phi *= M_PI / 180;
                            pl.planet.setPosition(pos2 + Vector2f(cos(phi), sin(phi)) * float(20 + (rand() % int(r2))));
                            pl.SetRadius(10 + (rand() % int(r2 / 2 - 9)));

                            for (int newP = 0; newP < newPlanets.size(); newP++)
                            {
                                if (Planet::Dist(newPlanets[newP].GetPosition(), pl.GetPosition()) <= newPlanets[newP].GetRadius() + pl.GetRadius())
                                    planetOverriding = true;
                            }
                        } while (planetOverriding);

                        plVel = pl.GetPosition() - pos2;
                        plVel /= Planet::Dist(plVel);
                        pl.SetVelocity(plVel / float(30 + rand() % 5));
                        newPlanets.push_back(pl);
                    }

                    planets.erase(planets.begin() + i);
                    planets.erase(planets.begin() + j);
                    xValues.erase(xValues.begin() + i);
                    xValues.erase(xValues.begin() + j);
                }
                else if (r1 > 40 && r2 <= 40) // planet i absorb planet j
                {
                    SetPlanetRadius(i, std::cbrt(std::pow(r1, 3) + 64000));
                    SetPlanetDensity(i, planets[i].GetDensity() + 0.1 + float((rand() % 10)) / 10.0);

                    planets.erase(planets.begin() + j);
                    xValues.erase(xValues.begin() + j);
                }
                else if (r2 > 40 && r1 <= 40) // planet j absorb planet i
                {
                    SetPlanetRadius(j, std::cbrt(std::pow(r2, 3) + 64000));
                    SetPlanetDensity(j, planets[j].GetDensity() + 0.1 + float((rand() % 10)) / 10.0);

                    planets.erase(planets.begin() + i);
                    xValues.erase(xValues.begin() + i);
                }
                else // both planets too small
                {
                    planets.erase(planets.begin() + i);
                    planets.erase(planets.begin() + j);
                    xValues.erase(xValues.begin() + i);
                    xValues.erase(xValues.begin() + j);
                }
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
            xValues.push_back(0.0f);
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
    xValues.erase(xValues.begin() + index);
    StopExpanding(true);
}

/**
    Remove every planet from the simulation.
*/
void PlanetSystem::ClearEverything()
{
    for (int i = explosionParticles.size() - 1; i >= 0; i--)
    {
        explosionParticles.erase(explosionParticles.begin() + i);
    }
    for (int i = planets.size() - 1; i >= 0; i--)
    {
        planets.erase(planets.begin() + i);
        xValues.erase(xValues.begin() + i);
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
    GravitationalConst = value / 5e4;
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
    Drawing every particle and planet.
*/
void PlanetSystem::draw(RenderTarget& target, RenderStates states) const
{
    states.texture = NULL;

    // draw the particles
    for (int i = 0; i < backgroundParticles.size(); i++)
    {
        target.draw(backgroundParticles[i], states);
    }
    for (int i = 0; i < explosionParticles.size(); i++)
    {
        target.draw(explosionParticles[i], states);
    }

    // draw the planets
    for (int i = 0; i < planets.size(); i++)
    {
        target.draw(planets[i], states);
    }
}
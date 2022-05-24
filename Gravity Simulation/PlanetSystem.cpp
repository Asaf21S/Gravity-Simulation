#include "PlanetSystem.h"

/**
    PlanetSystem constructor.
    @param windowSize - the size of the window.
*/
PlanetSystem::PlanetSystem(Vector2u windowSize) :
    windowSize(windowSize),
    isSparkling(false),
    firstHalfSparkle(true),
    spark(PrimitiveType::TriangleFan, 10),
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

    if (!backgroundTexture.loadFromFile("Textures/stars_milky_way.jpg"))
        std::cerr << "Unable to load stars_milky_way texture." << std::endl;
    backgroundSprite.setTexture(backgroundTexture);

    float rad = 3;
    spark[0].position = Vector2f(0, 0);
    spark[2].position = Vector2f(rad, rad);
    spark[4].position = Vector2f(-rad, rad);
    spark[6].position = -Vector2f(rad, rad);
    spark[8].position = -Vector2f(-rad, rad);

    spark[1].color = Color::Cyan;
    spark[3].color = Color::Cyan;
    spark[5].color = Color::Cyan;
    spark[7].color = Color::Cyan;
    spark[9].color = Color::Cyan;

    if (!planetShadowTexture.loadFromFile("Textures/planet.jpg"))
        std::cerr << "Unable to load planet shadow texture." << std::endl;
    planetShadowSprite.setTexture(planetShadowTexture);

    /**
    * Source: https://www.solarsystemscope.com/textures/
    * 
    * Textures in this pack are based on NASA elevation and imagery data.
    * Colors and shades of the textures are tuned according to true-color
    * photos made by Messenger, Viking and Cassini spacecrafts, and, of
    * course, the Hubble Space Telescope.
    */
    if (!planetTextures[0].loadFromFile("Textures/earth.jpg"))
        std::cerr << "Unable to load earth texture." << std::endl;
    if (!planetTextures[1].loadFromFile("Textures/jupiter.jpg"))
        std::cerr << "Unable to load jupiter texture." << std::endl;
    if (!planetTextures[2].loadFromFile("Textures/mars.jpg"))
        std::cerr << "Unable to load mars texture." << std::endl;
    if (!planetTextures[3].loadFromFile("Textures/mercury.jpg"))
        std::cerr << "Unable to load mercury texture." << std::endl;
    if (!planetTextures[4].loadFromFile("Textures/moon.jpg"))
        std::cerr << "Unable to load moon texture." << std::endl;
    if (!planetTextures[5].loadFromFile("Textures/neptune.jpg"))
        std::cerr << "Unable to load neptune texture." << std::endl;
    if (!planetTextures[6].loadFromFile("Textures/saturn.jpg"))
        std::cerr << "Unable to load saturn texture." << std::endl;
    if (!planetTextures[7].loadFromFile("Textures/sun.jpg"))
        std::cerr << "Unable to load sun texture." << std::endl;
    if (!planetTextures[8].loadFromFile("Textures/uranus.jpg"))
        std::cerr << "Unable to load uranus texture." << std::endl;
    if (!planetTextures[9].loadFromFile("Textures/venus.jpg"))
        std::cerr << "Unable to load venus texture." << std::endl;

    for (int i = 0; i < TEXTURES_AMOUNT; i++)
    {
        planetSprites[i].setTexture(planetTextures[i]);
    }
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
        // if the click was inside a planet -> edit this planet
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
            planets.push_back(Planet(mousePos, -1));
            finalTex.push_back(Texture());
            planets.back().SetTexture(finalTex.back());
            float initialXVal = rand() % planetTextures[planets.back().planetSurfaceInd].getSize().x / 1.5;
            xValues.push_back(initialXVal);
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
void PlanetSystem::Update(Time elapsed, Menu& menu)
{
    // update particles
    if (!isPaused)
    {
        // sparkle-effect
        if (isSparkling)
        {
            Sparkle(elapsed);
        }
        else if (rand() % 40 == 0)
        {
            isSparkling = true;
            Vector2f pos = Vector2f(rand() % windowSize.x, rand() % windowSize.y);
            for (int i = 0; i < spark.getVertexCount(); i++)
            {
                spark[i].position += pos;
            }
        }

        // delete explosion particles that outside the frame
        for (int i = 0; i < explosionParticles.size(); i++)
        {
            explosionParticles[i].Update(elapsed);
            if (!explosionParticles[i].IsVisible())
            {
                explosionParticles.erase(explosionParticles.begin() + i);
                break;
            }
        }

        // rotate planets' texture
        for (int i = 0; i < planets.size(); i++)
        {
            xValues[i] += planets[i].spinSpeed;
            float texRectX = planetTextures[planets[i].planetSurfaceInd].getSize().x / 1.5;
            if (xValues[i] >= texRectX) xValues[i] -= texRectX;
        }
    }

    // update planets
    if (expandPlanet) Expand();
    float len, mag;
    std::vector<Planet> newPlanets;
    for (int i = 0; i < planets.size(); i++)
    {
        // removing the planet if it is too far
        if (planets[i].GetPosition().x < -2 * int(windowSize.x) || planets[i].GetPosition().x > 3 * int(windowSize.x) ||
            planets[i].GetPosition().y < -2 * int(windowSize.y) || planets[i].GetPosition().y > 3 * int(windowSize.y))
        {
            RemovePlanet(i);
            continue;
        }

        // compute gravitational force
        planets[i].acceleration.x = 0;
        planets[i].acceleration.y = 0;
        for (int j = 0; j < planets.size(); j++)
        {
            if (i != j)
            {
                Vector2f force = planets[j].GetPosition() - planets[i].GetPosition();
                len = Planet::Dist(force);
                force /= len;
                mag = GravitationalConst * planets[i].GetMass() * planets[j].GetMass() / pow(len, 2);
                force *= mag;
                planets[i].AddForce(force);
            }
        }
        // need to call planets[i].SetArrowVisibility before calling planets[i].Update

        // set the planet texture
        float r1 = planets[i].GetRadius();
        int currentSurface = planets[i].planetSurfaceInd;

        planetSprites[currentSurface].setTextureRect(IntRect(xValues[i], planetTextures[currentSurface].getSize().y / 2 - r1, 2 * r1, 2 * r1));
        planetSprites[currentSurface].setOrigin(Vector2f(planetSprites[currentSurface].getLocalBounds().width / 2, planetSprites[currentSurface].getLocalBounds().height / 2));
        planetSprites[currentSurface].setRotation(planets[i].spinAngle);
        planetSprites[currentSurface].setPosition(Vector2f(r1, r1));
        planetShadowSprite.setScale(2 * r1 / planetShadowTexture.getSize().x, 2 * r1 / planetShadowTexture.getSize().y);
        RenderTexture renderTexture;
        renderTexture.create(2 * r1, 2 * r1);
        renderTexture.draw(planetSprites[currentSurface]);
        if (currentSurface != 7) renderTexture.draw(planetShadowSprite, sf::RenderStates(sf::BlendMultiply));
        renderTexture.display();
        std::list<Texture>::iterator texPtrs = GetTextureIterator(i);
        *texPtrs = renderTexture.getTexture();
        planets[i].planet.setTextureRect(IntRect(0, 0, (*texPtrs).getSize().x, (*texPtrs).getSize().y));

        planets[i].Update(elapsed, isPaused);

        // updating the edit menu:
        menu.UpdateVelocitySliders(i, planets[i].GetVelDirection(), planets[i].GetVelMagnitude());

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
                int jSurface = planets[j].planetSurfaceInd;

                int particleAmount = 5 + (rand() % 6);
                for (int p = 0; p < particleAmount; p++) // add explosion particles to one side
                {
                    float radius = 3 + (rand() % 3);
                    float degree = atan2(normal.y, normal.x) * 180 / M_PI;
                    degree += (rand() % 21) - 10;
                    degree *= M_PI / 180.0f;
                    float mag = minVelMag + (rand() % maxVelMag);
                    Vector2f vel = Vector2f(mag * cos(degree), mag * sin(degree));
                    int particleSurface = float(p) / float(particleAmount) > 0.5 ? currentSurface : jSurface;
                    explosionParticles.push_back(Particle(particlePosition, radius, vel, particleSurface));
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
                    int particleSurface = float(p) / float(particleAmount) > 0.5 ? currentSurface : jSurface;
                    explosionParticles.push_back(Particle(particlePosition, radius, vel, particleSurface));
                }

                if (r1 > 40 && r2 > 40) // both planets big enough
                {
                    bool planetOverriding;
                    float phi;
                    Vector2f plVel(0.0f, 0.0f);

                    int amount = 3 + (rand() % 3);
                    for (int p = 0; p < amount; p++) // planet i explosion
                    {
                        Planet pl(Vector2f(0.0f, 0.0f), currentSurface);
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
                        Planet pl(Vector2f(0.0f, 0.0f), planets[j].planetSurfaceInd);
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

                    // in case we were on the edit menu of the planet that was exploded, going back to the main menu.
                    menu.SwitchMenus(*this, -1, std::pair<int, int>(i, j));

                    planets.erase(planets.begin() + i);
                    planets.erase(planets.begin() + j);
                    finalTex.erase(GetTextureIterator(i));
                    finalTex.erase(GetTextureIterator(j));
                    xValues.erase(xValues.begin() + i);
                    xValues.erase(xValues.begin() + j);
                }
                else if (r1 > 40 && r2 <= 40) // planet i absorb planet j
                {
                    SetPlanetRadius(i, std::cbrt(std::pow(r1, 3) + 64000));
                    menu.UpdatePlanetDisplay(planets[i], i);
                    SetPlanetDensity(i, planets[i].GetDensity() + 0.1 + float((rand() % 10)) / 10.0);

                    // in case we were on the edit menu of the planet that was absorbed, going back to the main menu.
                    menu.SwitchMenus(*this, -1, std::pair<int, int>(j, -1));

                    planets.erase(planets.begin() + j);
                    finalTex.erase(GetTextureIterator(j));
                    xValues.erase(xValues.begin() + j);
                }
                else if (r2 > 40 && r1 <= 40) // planet j absorb planet i
                {
                    SetPlanetRadius(j, std::cbrt(std::pow(r2, 3) + 64000));
                    menu.UpdatePlanetDisplay(planets[j], j);
                    SetPlanetDensity(j, planets[j].GetDensity() + 0.1 + float((rand() % 10)) / 10.0);

                    // in case we were on the edit menu of the planet that was absorbed, going back to the main menu.
                    menu.SwitchMenus(*this, -1, std::pair<int, int>(i, -1));

                    planets.erase(planets.begin() + i);
                    finalTex.erase(GetTextureIterator(i));
                    xValues.erase(xValues.begin() + i);
                }
                else // both planets are too small
                {
                    // in case we were on the edit menu of one of the planets, going back to the main menu.
                    menu.SwitchMenus(*this, -1, std::pair<int, int>(i, j));

                    planets.erase(planets.begin() + i);
                    planets.erase(planets.begin() + j);
                    finalTex.erase(GetTextureIterator(i));
                    finalTex.erase(GetTextureIterator(j));
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
            finalTex.push_back(Texture());
            newPlanets[i].SetTexture(finalTex.back());
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
    {
        planets[setVelocityInd].SetVelocity(Vector2f(0.0f, 0.0f));
        planets[setVelocityInd].velDirection = mousePos - planets[setVelocityInd].GetPosition();
    }
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
    finalTex.erase(GetTextureIterator(index));
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
        finalTex.erase(GetTextureIterator(i));
        xValues.erase(xValues.begin() + i);
    }
    setVelocityInd = -1;
}

void PlanetSystem::Sparkle(Time elapsed)
{
    sparklingTime += elapsed;
    if (sparklingTime.asSeconds() > 1)
    {
        sparklingTime = Time::Zero;
        isSparkling = false;
        firstHalfSparkle = true;

        Vector2f pos = spark[0].position;
        for (int i = 0; i < spark.getVertexCount(); i++)
        {
            spark[i].position -= pos;
        }
    }
    else if (sparklingTime.asSeconds() > 0.5)
    {
        firstHalfSparkle = false;
    }

    if (isSparkling)
    {
        float progress = firstHalfSparkle ? sparklingTime.asSeconds() : 1 - sparklingTime.asSeconds();
        progress *= 20;

        spark[1].position = spark[0].position + Vector2f(progress, 0);
        spark[3].position = spark[0].position + Vector2f(0, progress);
        spark[5].position = spark[0].position - Vector2f(progress, 0);
        spark[7].position = spark[0].position - Vector2f(0, progress);
        spark[9].position = spark[0].position + Vector2f(progress, 0);
    }
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

void PlanetSystem::ReadyTemplate()
{
    ClearEverything();

    // creating the sun
    planets.push_back(Planet(Vector2f(windowSize) / 2.0f, 7));
    finalTex.push_back(Texture());
    planets.back().SetTexture(finalTex.back());
    float initialXVal = rand() % planetTextures[7].getSize().x / 1.5;
    xValues.push_back(initialXVal);
    planets.back().SetRadius(100);
    planets.back().LockPlanet(false);
    planets.back().isLocked = true;

    // creating the earth
    float distance = 300;
    planets.push_back(Planet(Vector2f(windowSize.x / 2.0f + distance, windowSize.y / 2.0f), 0));
    finalTex.push_back(Texture());
    planets.back().SetTexture(finalTex.back());
    initialXVal = rand() % planetTextures[0].getSize().x / 1.5;
    xValues.push_back(initialXVal);
    planets.back().SetRadius(30);
    // according to the Orbital Speed Formula: v = sqrt(G * Mcentral / R)
    float v = sqrt(GravitationalConst * (planets[0].GetMass() + planets[1].GetMass()) / distance);
    planets.back().SetVelocity(Vector2f(0, v));
    if (showAcc) planets.back().ToggleArrowVisibility(false);
    if (showVel) planets.back().ToggleArrowVisibility(true);
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
const Planet& PlanetSystem::SetPlanetRadius(int index, float radius)
{
    CheckIndex(index);
    planets[index].SetRadius(radius);
    return planets[index];
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
    planets[index].UpdateMass();
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
    planets[index].velDirection = Vector2f(cos(dir), sin(dir));
}

/**
    Set the velocity magnitude of a specific planet.
    @param index - the index of the planet to change the velocity magnitude to.
    @param mag - the magnitude which the velocity will be set to.
*/
void PlanetSystem::SetPlanetVelMag(int index, float mag)
{
    CheckIndex(index);
    float dir = planets[index].GetVelDirection();
    dir = 360 - dir;
    dir *= M_PI / 180.0f;
    planets[index].SetVelocity(Vector2f(mag * cos(dir), mag * sin(dir)));
}

int PlanetSystem::ChangeSurface(int index, bool isNext)
{
    CheckIndex(index);
    planets[index].planetSurfaceInd = (planets[index].planetSurfaceInd + TEXTURES_AMOUNT + (isNext ? 1 : -1)) % TEXTURES_AMOUNT;
    return planets[index].planetSurfaceInd;
}

void PlanetSystem::ToggleLock(int index)
{
    CheckIndex(index);
    planets[index].isLocked = !planets[index].isLocked;
    if (planets[index].isLocked) planets[index].LockPlanet(false);
    else
    {
        if (showAcc) planets[index].ToggleArrowVisibility(false);
        if (showVel) planets[index].ToggleArrowVisibility(true);
    }
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

std::list<Texture>::iterator PlanetSystem::GetTextureIterator(int index)
{
    std::list<Texture>::iterator texPtrs = finalTex.begin();
    std::advance(texPtrs, index);
    return texPtrs;
}

/**
    Drawing every particle and planet.
*/
void PlanetSystem::draw(RenderTarget& target, RenderStates states) const
{
    states.texture = NULL;

    // draw the particles
    target.draw(backgroundSprite, states);
    if (isSparkling) target.draw(spark);
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
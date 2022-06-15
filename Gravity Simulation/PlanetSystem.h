#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include "Planet.h"
#include "Particle.h"
#include "Menu.h"
using namespace sf;

const float MOUSE_MASS = 268082.5731; // (1.0 * 4.0 * M_PI * pow(40, 3)) / 3.0;
class Menu;

class PlanetSystem : public Drawable, public Transformable
{
    Vector2u windowSize;
    std::vector<Planet> planets;
    bool isSparkling;
    bool firstHalfSparkle;
    Time sparklingTime;
    VertexArray spark;
    std::vector<Particle> explosionParticles;
    bool isPaused;
    float GravitationalConst;
    bool showVel, showAcc;
    bool showTrail;
    bool expandPlanet;
    int setVelocityInd;
    float currentMaxR;
    int followerIndex;
    bool mouseForce;

    Texture backgroundTexture, planetShadowTexture, planetTextures[TEXTURES_AMOUNT];
    Sprite backgroundSprite, planetShadowSprite, planetSprites[TEXTURES_AMOUNT];
    std::list<Texture> finalTex;
    std::vector<float> xValues;

    void CheckIndex(int index);
    std::list<Texture>::iterator GetTextureIterator(int index);
    virtual void draw(RenderTarget& target, RenderStates states) const;

public:
    PlanetSystem(Vector2u windowSize);
    void MouseClicked(Vector2f mousePos, Menu& menu);
    int GetAmount();
    void Expand(int index = -1);
    void Update(Time elapsed, Menu& menu, Vector2f mousePos);
    void RemovePlanet(int index = -1);
    void ClearEverything();
    void Sparkle(Time elapsed);
    bool GetState();
    void ToggleState();
    void ToggleArrowVisibility(bool isVel);
    void ToggleMouseForce();
    void ReadyTemplate();
    void CreateFollower(Vector2f mousePos);
    bool TrackMouse();
    void UpdateFromMouse(Vector2f mousePos);
    void StopExpanding(bool isRemoved, int index = -1);
    void SetGConst(float value);
    const Planet& Editing(int index);
    void RemoveOutlines();
    const Planet& SetPlanetRadius(int index, float radius);
    void SetPlanetDensity(int index, float density);
    void SetPlanetVelDir(int index, float dir);
    void SetPlanetVelMag(int index, float mag);
    int ChangeSurface(int index, bool isNext);
    void ToggleLock(int index);
};
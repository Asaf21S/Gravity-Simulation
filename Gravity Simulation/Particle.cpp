#include "Particle.h"

/**
    Particle constructor.
    @param position - the position of the particle center.
    @param radius - particle's radius.
    @param velocity - particle's velocity.
*/
Particle::Particle(Vector2f position, float radius, Vector2f velocity) :
    particle(radius),
    velocity(velocity)
{
    particle.setOrigin(Vector2f(particle.getRadius(), particle.getRadius()));
    particle.setPosition(position);
    particle.setFillColor(Color::White);
}

/**
    Update the particle on a new frame.
    The function updates the particle's life time and position.
    @param elapsed - the time passed since the last call.
*/
void Particle::Update(sf::Time elapsed)
{
    lifeTime += elapsed;
    particle.move(velocity);
}

/**
    Drawing the particle.
*/
void Particle::draw(RenderTarget& target, RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // our particle don't use a texture
    states.texture = NULL;

    // draw the particle
    target.draw(particle, states);
}
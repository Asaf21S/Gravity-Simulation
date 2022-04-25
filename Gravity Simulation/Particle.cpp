#include "Particle.h"

/**
    Particle constructor.
    @param position - the position of the particle center.
    @param radius - particle's radius.
    @param velocity - particle's velocity.
*/
Particle::Particle(Vector2f position, float radius, Vector2f velocity) :
    particle(radius),
    velocity(velocity),
    isSparkling(false),
    firstHalf(true)
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
void Particle::Update()
{
    particle.move(velocity);
}

void Particle::Sparkle(Time elapsed)
{
    sparklingTime += elapsed;
    if (sparklingTime.asSeconds() > 1)
    {
        sparklingTime = Time::Zero;
        isSparkling = false;
        firstHalf = true;
    }
    else if (sparklingTime.asSeconds() > 0.5)
    {
        firstHalf = false;
    }
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

    if (isSparkling)
    {
        VertexArray spark(PrimitiveType::TriangleFan, 10);
        Vector2f center = particle.getPosition();
        float rad = particle.getRadius() / sqrt(2);
        float progress = firstHalf ? sparklingTime.asSeconds() : 1 - sparklingTime.asSeconds();
        progress *= 20;

        spark[0].position = center;
        spark[1].position = center + Vector2f(progress, 0);
        spark[2].position = center + Vector2f(rad, rad);
        spark[3].position = center + Vector2f(0, progress);
        spark[4].position = center + Vector2f(-rad, rad);
        spark[5].position = center - Vector2f(progress, 0);
        spark[6].position = center - Vector2f(rad, rad);
        spark[7].position = center - Vector2f(0, progress);
        spark[8].position = center - Vector2f(-rad, rad);
        spark[9].position = spark[1].position;

        spark[1].color = Color::Cyan;
        spark[3].color = Color::Cyan;
        spark[5].color = Color::Cyan;
        spark[7].color = Color::Cyan;
        spark[9].color = Color::Cyan;

        target.draw(spark, states);
    }

    // draw the particle
    target.draw(particle, states);
}
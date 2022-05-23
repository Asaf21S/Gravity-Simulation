#include "Particle.h"

/**
    Particle constructor.
    @param position - the position of the particle center.
    @param radius - particle's radius.
    @param velocity - particle's velocity.
*/
Particle::Particle(Vector2f position, float radius, Vector2f velocity, int surfaceIndex) :
    particle(radius),
    velocity(velocity)
{
    particle.setOrigin(Vector2f(particle.getRadius(), particle.getRadius()));
    particle.setPosition(position);

	switch (surfaceIndex)
	{
	case 0:
		color = Color(153, 255, 153);
		break;
	case 1:
		color = Color(255, 255, 255);
		break;
	case 2:
		color = Color(192, 192, 192);
		break;
	case 3:
		color = Color(224, 224, 224);
		break;
	case 4:
		color = Color(224, 224, 224);
		break;
	case 5:
		color = Color(153, 204, 255);
		break;
	case 6:
		color = Color(255, 229, 204);
		break;
	case 7:
		color = Color(255, 255, 153);
		break;
	case 8:
		color = Color(153, 255, 255);
		break;
	case 9:
		color = Color(255, 204, 153);
		break;
	default:
		color = Color(255, 255, 255);
		break;
	}
    particle.setFillColor(color);

    fadingTime = float(10 + (rand() % 15)) / 10.0f;
}

bool Particle::IsVisible()
{
    return lifeTime.asSeconds() < fadingTime;
}

/**
    Update the particle on a new frame.
    The function updates the particle's life time and position.
    @param elapsed - the time passed since the last call.
*/
void Particle::Update(Time elapsed)
{
    particle.move(velocity);
    lifeTime += elapsed;
	float percent = 1 - (lifeTime.asSeconds() / fadingTime);
    Color c = Color(color.r * percent, color.g * percent, color.b * percent);
    particle.setFillColor(c);
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
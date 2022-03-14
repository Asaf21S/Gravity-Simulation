#include "Planet.h"

Planet::Planet(Vector2f position) :
    planet(0),
    density(1.f),
    acceleration(0, 0),
    velArrow(PrimitiveType::TriangleFan, 7),
    showVelArrow(false)
{
    planet.setPosition(position);
    UpdateMass();

    velArrow[0].color = Color(153, 0, 0);
    velArrow[1].color = Color(163, 15, 5);
    velArrow[2].color = Color(163, 15, 5);
    velArrow[3].color = Color(255, 153, 51);
    velArrow[4].color = Color(255, 153, 51);
    velArrow[5].color = Color(163, 15, 5);
    velArrow[6].color = Color(163, 15, 5);
}

void Planet::SetArrow(Vector2f mouse)
{
    Vector2f center = GetPosition();
    float radius = planet.getRadius();
    float distance = sqrt(pow(mouse.x - center.x, 2) + pow(mouse.y - center.y, 2));
    if (distance < radius + ARROW_OFFSET) return;
    showVelArrow = true;

    Vector2f normal = Vector2f(mouse.y - center.y, center.x - mouse.x);
    float len = sqrt(pow(normal.x, 2) + pow(normal.y, 2));
    normal /= len;

    Vector2f circleEdge = Vector2f((distance - (radius + ARROW_OFFSET)) * center.x + (radius + ARROW_OFFSET) * mouse.x, (distance - (radius + ARROW_OFFSET)) * center.y + (radius + ARROW_OFFSET) * mouse.y);
    circleEdge /= distance;

    Vector2f arrowEdge = Vector2f(center.x + 9 * mouse.x, center.y + 9 * mouse.y);
    arrowEdge /= 10.0f;

    velArrow[0].position = mouse;
    velArrow[1].position = arrowEdge + 16.0f * normal;
    velArrow[2].position = arrowEdge + 3.0f * normal;
    velArrow[3].position = circleEdge + 3.0f * normal;
    velArrow[4].position = circleEdge - 3.0f * normal;
    velArrow[5].position = arrowEdge - 3.0f * normal;
    velArrow[6].position = arrowEdge - 16.0f * normal;

    velocity = mouse - circleEdge;
    velocity /= ARROW_LENGTH_TO_VELOCITY;
}

void Planet::UpdateArrow()
{
    Vector2f arrow = velocity * ARROW_LENGTH_TO_VELOCITY;
    float length = sqrt(pow(arrow.x, 2) + pow(arrow.y, 2));
    float distance = length + planet.getRadius() + ARROW_OFFSET;
    SetArrow(GetPosition() + (arrow / length * distance));
}

void Planet::SetVelocity(Vector2f vel)
{
    velocity = vel;
}

void Planet::Expand()
{
    float factor = 1;
    planet.setRadius(planet.getRadius() + factor);
    planet.move(-factor, -factor);
    UpdateMass();
}

Vector2f Planet::GetPosition()
{
    return Vector2f(planet.getPosition().x + planet.getRadius(), planet.getPosition().y + planet.getRadius());
}

float Planet::GetRadius()
{
    return planet.getRadius();
}

float Planet::GetMass()
{
    return mass;
}

void Planet::AddForce(Vector2f force)
{
    acceleration += force;
}

void Planet::Update(sf::Time elapsed, bool isPaused, bool showVelArrow)
{
    this->showVelArrow = showVelArrow;
    if (showVelArrow) UpdateArrow();
    if (!isPaused)
    {
        lifeTime += elapsed;
        planet.move(velocity);
        velocity += acceleration / mass;
    }

    acceleration.x = 0;
    acceleration.y = 0;
}

void Planet::UpdateMass()
{
    // mass = density * volume
    // volume = (4/3)πr^3
    mass = (density * 4.0 * M_PI * pow(planet.getRadius(), 3)) / 3.0;
}

void Planet::draw(RenderTarget& target, RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // our planet don't use a texture
    states.texture = NULL;

    // draw the planet
    target.draw(planet, states);

    // draw the velocity arrow
    if (showVelArrow)
        target.draw(velArrow, states);
}
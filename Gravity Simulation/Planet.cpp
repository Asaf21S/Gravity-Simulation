#include "Planet.h"

Planet::Planet(Vector2f position) :
    planet(0),
    density(1.f),
    velocity(0, 0),
    velArrow(PrimitiveType::TriangleFan, 7),
    showVelArrow(false),
    velTooSmall(true),
    acceleration(0, 0),
    accArrow(PrimitiveType::TriangleFan, 7),
    showAccArrow(false),
    accTooSmall(true)
{
    planet.setPosition(position);
    planet.setOutlineColor(Color::Blue);
    UpdateMass();

    velArrow[0].color = Color(153, 0, 0);
    velArrow[3].color = Color(255, 153, 51);
    velArrow[4].color = Color(255, 153, 51);

    accArrow[0].color = Color(0, 153, 153);
    accArrow[3].color = Color(255, 255, 255);
    accArrow[4].color = Color(255, 255, 255);
}

void Planet::SetArrow(Vector2f arrowPoint, bool isVel)
{
    Vector2f center = GetPosition();
    float radius = planet.getRadius();
    float distance = Dist(arrowPoint, center);
    float lineLength = distance - (radius + ARROW_OFFSET);
    if (lineLength < 12.0f)
    {
        if (isVel) velTooSmall = true;
        else accTooSmall = true;
        return;
    }
    if (isVel) velTooSmall = false;
    else accTooSmall = false;

    Vector2f normal = Vector2f(arrowPoint.y - center.y, center.x - arrowPoint.x);
    float len = Dist(normal);
    normal /= len;

    Vector2f circleEdge = Vector2f(lineLength * center.x + (radius + ARROW_OFFSET) * arrowPoint.x, lineLength * center.y + (radius + ARROW_OFFSET) * arrowPoint.y);
    circleEdge /= distance;

    float arrowLength = lineLength <= 56.0f ? lineLength / 2.0f : 28.0f;
    float arrowWidth = arrowLength / 1.732; // approx. sqrt(3)
    float lengthRatio = arrowLength / lineLength;

    Vector2f arrowEdge = Vector2f(lengthRatio * circleEdge.x + (1 - lengthRatio) * arrowPoint.x, lengthRatio * circleEdge.y + (1 - lengthRatio) * arrowPoint.y);

    float lineWidth = arrowWidth * 0.1875f; // 3/16
    if (lineWidth < 0.5f) lineWidth = 0.5f;

    Vector2f normalToLine = lineWidth * normal, normalToArrow = arrowWidth * normal;
    
    if (isVel)
    {
        velArrow[0].position = arrowPoint;
        velArrow[1].position = arrowEdge + normalToArrow;
        velArrow[2].position = arrowEdge + normalToLine;
        velArrow[3].position = circleEdge + normalToLine;
        velArrow[4].position = circleEdge - normalToLine;
        velArrow[5].position = arrowEdge - normalToLine;
        velArrow[6].position = arrowEdge - normalToArrow;

        Color arrowCol = Color( (1 - lengthRatio) * velArrow[0].color.r + lengthRatio * velArrow[3].color.r,
                                (1 - lengthRatio) * velArrow[0].color.g + lengthRatio * velArrow[3].color.g,
                                (1 - lengthRatio) * velArrow[0].color.b + lengthRatio * velArrow[3].color.b );
        velArrow[1].color = arrowCol;
        velArrow[2].color = arrowCol;
        velArrow[5].color = arrowCol;
        velArrow[6].color = arrowCol;

        velocity = arrowPoint - circleEdge;
        velocity /= ARROW_LENGTH_TO_VELOCITY;
    }
    else
    {
        accArrow[0].position = arrowPoint;
        accArrow[1].position = arrowEdge + normalToArrow;
        accArrow[2].position = arrowEdge + normalToLine;
        accArrow[3].position = circleEdge + normalToLine;
        accArrow[4].position = circleEdge - normalToLine;
        accArrow[5].position = arrowEdge - normalToLine;
        accArrow[6].position = arrowEdge - normalToArrow;

        Color arrowCol = Color( (1 - lengthRatio) * accArrow[0].color.r + lengthRatio * accArrow[3].color.r,
                                (1 - lengthRatio) * accArrow[0].color.g + lengthRatio * accArrow[3].color.g,
                                (1 - lengthRatio) * accArrow[0].color.b + lengthRatio * accArrow[3].color.b );
        accArrow[1].color = arrowCol;
        accArrow[2].color = arrowCol;
        accArrow[5].color = arrowCol;
        accArrow[6].color = arrowCol;

        acceleration = arrowPoint - circleEdge;
        acceleration /= ARROW_LENGTH_TO_ACCELERATION;
    }
}

void Planet::UpdateArrow(bool isVel)
{
    Vector2f arrow;
    if (isVel) arrow = velocity * ARROW_LENGTH_TO_VELOCITY;
    else arrow = acceleration * ARROW_LENGTH_TO_ACCELERATION;
    float length = Dist(arrow);
    //if (length = 0) SetArrow(GetPosition() + (arrow / length * distance), isVel);
    float distance = length + planet.getRadius() + ARROW_OFFSET;
    SetArrow(GetPosition() + (arrow / length * distance), isVel);
}

void Planet::SetVelocity(Vector2f vel)
{
    velocity = vel;
}

void Planet::SetArrowVisibility(bool isVel)
{
    if (isVel) showVelArrow = !showVelArrow;
    else showAccArrow = !showAccArrow;
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

void Planet::SetRadius(float radius)
{
    planet.move(Vector2f(planet.getRadius() - radius, planet.getRadius() - radius));
    planet.setRadius(radius);
    UpdateMass();
}

float Planet::GetRadius() const
{
    return planet.getRadius();
}

float Planet::GetDensity() const
{
    return density;
}

float Planet::GetVelDirection() const
{
    float degree = atan2(velocity.y, velocity.x) * 180 / M_PI;
    if (degree < 0) degree += 360;
    degree = 360 - degree;
    return degree;
}

float Planet::GetVelMagnitude() const
{
    return Dist(velocity);
}

float Planet::GetMass()
{
    return mass;
}

void Planet::AddForce(Vector2f force)
{
    acceleration += force;
}

void Planet::Update(sf::Time elapsed, bool isPaused)
{
    // acceleration is currently the sum of all forces, so according to the equation - F=ma, we need to divide by the mass of the planet
    acceleration /= mass;
    if (!isPaused)
    {
        lifeTime += elapsed;
        planet.move(velocity);
        velocity += acceleration;
    }
    if (showVelArrow) UpdateArrow(true);
    if (showAccArrow) UpdateArrow(false);
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
    if (showVelArrow && !velTooSmall)
        target.draw(velArrow, states);

    // draw the acceleration arrow
    if (showAccArrow && !accTooSmall)
        target.draw(accArrow, states);
}

float Planet::Dist(Vector2f p1, Vector2f p2)
{
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}
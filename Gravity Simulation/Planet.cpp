#include "Planet.h"

/**
    Planet constructor.
    @param position - the position of the planet center.
*/
Planet::Planet(Vector2f position, int surfaceIndex) : // -1 for random
    planet(0),
    density(1.f),
    velocity(0, 0),
    velArrow(PrimitiveType::TriangleFan, 7),
    showVelArrow(false),
    velTooSmall(true),
    velDirection(0, 0),
    acceleration(0, 0),
    accArrow(PrimitiveType::TriangleFan, 7),
    showAccArrow(false),
    accTooSmall(true),
    isLocked(false)
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
    
    if (surfaceIndex == -1)
        planetSurfaceInd = rand() % TEXTURES_AMOUNT; //texind++ % TEXTURES_AMOUNT;
    else
        planetSurfaceInd = surfaceIndex;
    spinAngle = rand() % 360;
    spinSpeed = float(5 + (rand() % 15)) / 10;
}

/**
    Update the velocity or acceleration arrow and vector for a given edge point.
    @param arrowPoint - the position of the edge of the arrow.
    @param isVel - whether to update velocity (true) or acceleration (false).
*/
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
        if (velocity.x != 0 || velocity.y != 0)
            velDirection = velocity;
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

/**
    Update the velocity or acceleration arrow for the current vector.
    @param isVel - whether to update velocity (true) or acceleration (false).
*/
void Planet::UpdateArrow(bool isVel)
{
    Vector2f arrow;
    if (isVel) arrow = velocity * ARROW_LENGTH_TO_VELOCITY;
    else arrow = acceleration * ARROW_LENGTH_TO_ACCELERATION;
    float length = Dist(arrow);
    if (length == 0) SetArrow(GetPosition(), isVel);
    else
    {
        float distance = length + planet.getRadius() + ARROW_OFFSET;
        SetArrow(GetPosition() + (arrow / length * distance), isVel);
    }
}

/**
    Set the velocity vector.
    @param vel - the vector which the velocity will be set to.
*/
void Planet::SetVelocity(Vector2f vel)
{
    velocity = vel;
    if (velocity.x != 0 || velocity.y != 0)
        velDirection = velocity;
}

/**
    Toggle the velocity or acceleration arrow visibility.
    @param isVel - for velocity (true), for acceleration (false).
*/
void Planet::ToggleArrowVisibility(bool isVel)
{
    if (isVel) showVelArrow = !showVelArrow;
    else showAccArrow = !showAccArrow;
}

void Planet::LockPlanet(bool display, Vector2f position)
{
    if (display)
    {
        planet.setScale(50 / GetRadius(), 50 / GetRadius());
        position -= Vector2f(50.0f, 50.0f);
        planet.setPosition(position);
        planet.setOutlineThickness(0);
    }
    velocity = Vector2f(0.0f, 0.0f);
    showVelArrow = false;
    showAccArrow = false;
}

/**
    Increase the planet radius by factor = 1.
*/
void Planet::Expand()
{
    float factor = 1;
    planet.setRadius(planet.getRadius() + factor);
    planet.move(-factor, -factor);
    UpdateMass();
}

void Planet::SetPosition(Vector2f position)
{
    planet.setPosition(position - Vector2f(GetRadius(), GetRadius()));
}

/**
    Get the position of the center of the planet.
*/
Vector2f Planet::GetPosition()
{
    return Vector2f(planet.getPosition().x + planet.getRadius(), planet.getPosition().y + planet.getRadius());
}

void Planet::SetTexture(Texture& tex)
{
    planet.setTexture(&tex);
}

/**
    Set the radius of the planet.
    @param radius - the value which the radius will be set to.
*/
void Planet::SetRadius(float radius)
{
    planet.move(Vector2f(planet.getRadius() - radius, planet.getRadius() - radius));
    planet.setRadius(radius);
    UpdateMass();
}

/**
    Get the radius of the planet.
*/
float Planet::GetRadius() const
{
    return planet.getRadius();
}

/**
    Get the density of the planet.
*/
float Planet::GetDensity() const
{
    return density;
}

/**
    Get the angle of the velocity vector in degrees while:
     0   - east.
     90  - north.
     180 - west.
     270 - south.
*/
float Planet::GetVelDirection() const
{
    //std::cout << "A " << velocity.x << " " << velocity.y << std::endl;
    float degree = atan2(velDirection.y, velDirection.x) * 180 / M_PI;
    if (degree < 0) degree += 360;
    degree = 360 - degree;
    return degree;
}

/**
    Get the magnitude of the velocity vector, which is the vector length.
*/
float Planet::GetVelMagnitude() const
{
    return Dist(velocity);
}

int Planet::GetSurface() const
{
    return planetSurfaceInd;
}

bool Planet::IsLocked() const
{
    return isLocked;
}

/**
    Get the planet's mass.
*/
float Planet::GetMass()
{
    return mass;
}

/**
    Add a force to the planet's net force.
    @param force - the force vector to be applied on the planet.
*/
void Planet::AddForce(Vector2f force)
{
    acceleration += force;
}

/**
    Update the planet on a new frame. 
    The function updates the planet's life time, velocity, position and arrows.
    @param elapsed - the time passed since the last call.
    @param isPaused - whether the simulation is currently paused or not.
*/
void Planet::Update(sf::Time elapsed, bool isPaused)
{
    // acceleration is currently the sum of all forces, so according to the equation "F=ma", we need to divide by the mass of the planet
    acceleration /= mass;
    if (!isPaused)
    {
        lifeTime += elapsed;
        if (!isLocked)
        {
            velocity += acceleration;
            planet.move(velocity);
            
            if (velocity.x != 0 || velocity.y != 0)
                velDirection = velocity;
        }
    }
    
    if (showVelArrow) UpdateArrow(true);
    if (showAccArrow) UpdateArrow(false);
}

/**
    Update the planet's mass according to its density and radius, with the formulas:
     mass = density * volume
     volume = (4/3)πr^3
*/
void Planet::UpdateMass()
{
    mass = (density * 4.0 * M_PI * pow(planet.getRadius(), 3)) / 3.0;
}

/**
    Drawing the planet, and the velocity and acceleration arrows.
*/
void Planet::draw(RenderTarget& target, RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // our planet don't use a texture
    states.texture = NULL;

    // draw the planet
    target.draw(planet); // , states);

    // draw the velocity arrow
    if (showVelArrow && !velTooSmall)
        target.draw(velArrow, states);

    // draw the acceleration arrow
    if (showAccArrow && !accTooSmall)
        target.draw(accArrow, states);
}

/**
    Helper function that compute the distance between 2 points.
*/
float Planet::Dist(Vector2f p1, Vector2f p2)
{
    return std::hypotf(p1.x - p2.x, p1.y - p2.y);
}
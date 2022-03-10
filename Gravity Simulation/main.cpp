#include <iostream>
#include <SFML/Graphics.hpp>
#include "PlanetSystem.h"
#include "Planet.h"
#include "Slider.h"
#include "Button.h"
#include "CheckBox.h"

//using namespace std;
using namespace sf;
void ButtonIsPressed();

Planet p1(Vector2f(600, 100), 50);
Planet p2(Vector2f(700, 700), 40);
PlanetSystem planetSystem;

bool start = false;
bool isClicked = false;
bool expand = false;

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(VideoMode(1920, 1080), "Window Name", Style::Default, settings);
    window.setVerticalSyncEnabled(true); // for the application to run at the same frequency as the monitor's refresh rate

    p1.SetVelocity(Vector2f(1, 2));
    p2.SetVelocity(Vector2f(0, -1));
    planetSystem.AddPlanet(p1);
    planetSystem.AddPlanet(p2);

    Slider slider1(Vector2f(100, 100), 20, 450, 30);
    Slider slider2(Vector2f(100, 200), 0, 200, 0);
    Slider slider3(Vector2f(100, 300), 0, 100, 50);
    slider1.SetValue(235);

    Button btn(Vector2f(100, 400), 100, 50, ButtonIsPressed, "Label", Color::White, Color::Yellow);
    CheckBox cb(Vector2f(100, 550), "This is a CheckBox");

    sf::Clock clock;

    while (window.isOpen())
    {
        Event event;
        // while there are pending events
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                // add here things to do before the window is closed
                window.close();
                break;

            case Event::KeyPressed: // To achieve smooth movement with events, you must use a boolean that you set on KeyPressed and clear on KeyReleased
                break;

            case Event::LostFocus:
                //PauseSimulation.pause();
                break;

            case Event::GainedFocus:
                //ResumeSimulation.resume();
                break;

            case Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                    std::cout << "wheel type: vertical" << std::endl;
                else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                    std::cout << "wheel type: horizontal" << std::endl;
                else
                    std::cout << "wheel type: unknown" << std::endl;
                std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
                std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
                std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
                break;

            case Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    std::cout << "the right button was pressed" << std::endl;
                    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                    std::cout << "mouse y: " << event.mouseButton.y << std::endl;
                }
                break;

            default:
                break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // may return true even if your window is inactive
        {
            // left key is pressed: move our character
            //Planet.move(1.f, 0.f);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (!isClicked && planetSystem.pause)
            {
                expand = true;
                Planet p(Vector2f(Mouse::getPosition(window)), 0);
                planetSystem.AddPlanet(p);
            }
            isClicked = true;
        }
        else
        {
            isClicked = false;
            expand = false;
        }

        if (expand)
        {
            planetSystem.Expand();
        }

        sf::Time elapsed = clock.restart();
        planetSystem.Update(elapsed);
        slider1.Update(window);
        slider2.Update(window);
        slider3.Update(window);
        btn.Update(window);
        cb.Update(window);

        window.clear();
        window.draw(planetSystem);
        window.draw(slider1);
        window.draw(slider2);
        window.draw(slider3);
        window.draw(btn);
        window.draw(cb);
        window.display();
    }

    return 0;
}

void ButtonIsPressed()
{
    std::cout << "Button Click!" << std::endl;
    planetSystem.pause = !planetSystem.pause;
}

//sf::Vertex line[] =
//{
//    sf::Vertex(sf::Vector2f(0, 0)),
//    sf::Vertex(planet.getPosition())
//};
//line[1].color = Color::Black;
//
//target.draw(line, 2, sf::Lines);
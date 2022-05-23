#include <iostream>
#include <SFML/Graphics.hpp>
#include "PlanetSystem.h"
#include "Planet.h"
#include "Menu.h"

using namespace sf;

int main()
{
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(VideoMode(1920, 1080), "Window Name", Style::Default, settings);
    window.setVerticalSyncEnabled(true); // for the application to run at the same frequency as the monitor's refresh rate

    Font font;
    if (!font.loadFromFile("Fonts\\Neon.ttf"))
        std::cout << "Error loading font\n";
    Menu menu(font);
    PlanetSystem planetSystem(window.getSize());
    menu.Init(planetSystem, window.getSize().y);

    Clock clock;
    bool isClicked = false;

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
            case Event::LostFocus:
                //PauseSimulation.pause();
                break;
            case Event::GainedFocus:
                //ResumeSimulation.resume();
                break;
            default:
                break;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) // may return true even if your window is inactive
        {
            // left key is pressed: move our character
            //Planet.move(1.f, 0.f);
        }

        if (Mouse::isButtonPressed(Mouse::Left))
        {
            if (!isClicked)
            {
                // Mouse is first clicked
                if (menu.IsClickInside(Vector2f(Mouse::getPosition(window))))
                {
                    if (!planetSystem.TrackMouse())
                        menu.MouseClicked(Vector2f(Mouse::getPosition(window)), planetSystem);
                }
                else
                    planetSystem.MouseClicked(Vector2f(Mouse::getPosition(window)), menu);
            }
            isClicked = true;
        }
        else if (isClicked)
        {
            // Mouse click is released
            isClicked = false;
            planetSystem.StopExpanding(false);
            menu.MouseReleased();
        }

        window.clear();
        Time elapsed = clock.restart();

        planetSystem.Update(elapsed, menu);
        if (planetSystem.TrackMouse()) planetSystem.UpdateArrow(Vector2f(Mouse::getPosition(window)));
        if (menu.TrackMouse()) menu.UpdateSlider(float(Mouse::getPosition(window).x), planetSystem);
        if (menu.MenuIsCollapsed()) menu.CollapseMenu();
        menu.UpdateStats(elapsed, planetSystem.GetState(), planetSystem.GetAmount());

        window.draw(planetSystem);
        window.draw(menu);

        window.display();
    }

    return 0;
}

//Vertex line[] =
//{
//    Vertex(Vector2f(0, 0)),
//    Vertex(planet.getPosition())
//};
//line[1].color = Color::Black;
//
//target.draw(line, 2, Lines);
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
void CollapseMenu();
void CreateMenu(RenderWindow& window);

// menu items:
RectangleShape menuBackground;
Button collapseMenu(Vector2f(391, 35), 60, 60, CollapseMenu, "<", 64, Color(128, 128, 128, 200), Color(128, 128, 128, 200));
Slider slider1(Vector2f(100, 100), 20, 450, 30);
Slider slider2(Vector2f(100, 200), 0, 200, 0);
Slider slider3(Vector2f(100, 300), 0, 100, 50);
Button btn(Vector2f(100, 400), 100, 50, ButtonIsPressed, "Label", 32, Color::White, Color::Yellow);
CheckBox cb(Vector2f(100, 550), "This is a CheckBox");

Planet p1(Vector2f(600, 100), 50);
Planet p2(Vector2f(700, 700), 40);
PlanetSystem planetSystem;

bool start = false;
bool isClicked = false;
bool isCollapsed = false;
bool showMenuItems = true;
float maximumRadius;

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

    menuBackground.setSize(Vector2f(350, window.getSize().y));
    menuBackground.setFillColor(Color(32, 32, 32, 200));
    menuBackground.setOutlineColor(Color(128, 128, 128, 200));
    menuBackground.setOutlineThickness(6);
    slider1.SetValue(235);

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
            if (!isClicked && planetSystem.pause &&
                !menuBackground.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))) &&
                !collapseMenu.contains(Vector2f(Mouse::getPosition(window))))
            {
                Planet p(Vector2f(Mouse::getPosition(window)), 0);
                planetSystem.AddPlanet(p);
            }
            isClicked = true;
        }
        else
        {
            isClicked = false;
            planetSystem.StopExpanding();
        }

        window.clear();

        sf::Time elapsed = clock.restart();
        planetSystem.Update(elapsed);
        window.draw(planetSystem);
        CreateMenu(window);

        window.display();
    }

    return 0;
}

void CreateMenu(RenderWindow& window)
{
    int mouseX = Mouse::getPosition(window).x, mouseY = Mouse::getPosition(window).y;
    collapseMenu.Update(mouseX, mouseY);
    if (isCollapsed)
    {
        if (menuBackground.getSize().x > 40)
        {
            showMenuItems = false;
            menuBackground.setSize(Vector2f(menuBackground.getSize().x - 40, menuBackground.getSize().y));
            collapseMenu.move(-40.0f, 0);
        }
        else if (menuBackground.getSize().x > 0)
        {
            collapseMenu.move(-menuBackground.getSize().x, 0);
            menuBackground.setSize(Vector2f(0, menuBackground.getSize().y));
            collapseMenu.SetText(">");
        }
    }
    else
    {
        if (menuBackground.getSize().x < 350 - 40)
        {
            menuBackground.setSize(Vector2f(menuBackground.getSize().x + 40, menuBackground.getSize().y));
            collapseMenu.move(40.0f, 0);
        }
        else if (menuBackground.getSize().x < 350)
        {
            showMenuItems = true;
            collapseMenu.move(350 - menuBackground.getSize().x, 0);
            menuBackground.setSize(Vector2f(350, menuBackground.getSize().y));
            collapseMenu.SetText("<");
        }
    }
    window.draw(menuBackground);
    window.draw(collapseMenu);

    if (showMenuItems)
    {
        slider1.Update(mouseX, mouseY);
        slider2.Update(mouseX, mouseY);
        slider3.Update(mouseX, mouseY);
        btn.Update(mouseX, mouseY);
        cb.Update(mouseX, mouseY);
        window.draw(slider1);
        window.draw(slider2);
        window.draw(slider3);
        window.draw(btn);
        window.draw(cb);
    }
}

void ButtonIsPressed()
{
    std::cout << "Button Click!" << std::endl;
    planetSystem.pause = !planetSystem.pause;
}

void CollapseMenu()
{
    isCollapsed = !isCollapsed;
}

//sf::Vertex line[] =
//{
//    sf::Vertex(sf::Vector2f(0, 0)),
//    sf::Vertex(planet.getPosition())
//};
//line[1].color = Color::Black;
//
//target.draw(line, 2, sf::Lines);
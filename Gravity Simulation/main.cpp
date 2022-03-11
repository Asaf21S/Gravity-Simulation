#include <iostream>
#include <SFML/Graphics.hpp>
#include "PlanetSystem.h"
#include "Planet.h"
#include "Slider.h"
#include "Button.h"
#include "CheckBox.h"

//using namespace std;
using namespace sf;

const float MENU_WIDTH = 400.0f;
const float COLLAPSE_MENU_WIDTH = 60.0f;

void CreateMenu(RenderWindow& window);
void UpdateGConst(float value);
void StartPause();
void CollapseMenu();

// menu items:
RectangleShape menuBackground;
Text menuTitle;
Button collapseMenu(Vector2f(MENU_WIDTH + 6 + COLLAPSE_MENU_WIDTH / 2.0f + 5, COLLAPSE_MENU_WIDTH / 2.0f + 5), COLLAPSE_MENU_WIDTH, COLLAPSE_MENU_WIDTH, CollapseMenu, "<", 64, Color(128, 128, 128, 200), Color(128, 128, 128, 200));
Slider constG(Vector2f(MENU_WIDTH / 2, 150), 0, 100, 50, UpdateGConst);
CheckBox cb(250, "This is a CheckBox");
Button stateBtn(Vector2f(MENU_WIDTH / 2, 400), 100, 50, StartPause, "Start", 32, Color::White, Color::Magenta);

PlanetSystem planetSystem;

bool start = false;
bool isClicked = false;
bool isCollapsed = false;
bool showMenuItems = true;
float maximumRadius;

int main()
{
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(VideoMode(1920, 1080), "Window Name", Style::Default, settings);
    window.setVerticalSyncEnabled(true); // for the application to run at the same frequency as the monitor's refresh rate

    menuBackground.setSize(Vector2f(MENU_WIDTH, window.getSize().y));
    menuBackground.setFillColor(Color(32, 32, 32, 200));
    menuBackground.setOutlineColor(Color(128, 128, 128, 200));
    menuBackground.setOutlineThickness(6);
    Font font;
    if (!font.loadFromFile("Fonts\\Neon.ttf"))
        std::cout << "Error loading font\n";
    menuTitle.setString("Menu");
    menuTitle.setFont(font);
    menuTitle.setCharacterSize(64);
    menuTitle.setStyle(Text::Style::Underlined);
    menuTitle.setOrigin(menuTitle.getLocalBounds().left + menuTitle.getLocalBounds().width / 2.0f, menuTitle.getLocalBounds().top + menuTitle.getLocalBounds().height / 2.0f);
    menuTitle.setPosition(MENU_WIDTH / 2, 15 + menuTitle.getLocalBounds().height / 2.0f);

    Clock clock;

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
                if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel)
                    std::cout << "wheel type: vertical" << std::endl;
                else if (event.mouseWheelScroll.wheel == Mouse::HorizontalWheel)
                    std::cout << "wheel type: horizontal" << std::endl;
                else
                    std::cout << "wheel type: unknown" << std::endl;
                std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
                std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
                std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
                break;

            case Event::MouseButtonPressed:
                if (event.mouseButton.button == Mouse::Right)
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

        if (Keyboard::isKeyPressed(Keyboard::Left)) // may return true even if your window is inactive
        {
            // left key is pressed: move our character
            //Planet.move(1.f, 0.f);
        }

        if (Mouse::isButtonPressed(Mouse::Left))
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

        Time elapsed = clock.restart();
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
        if (menuBackground.getSize().x < MENU_WIDTH - 40)
        {
            menuBackground.setSize(Vector2f(menuBackground.getSize().x + 40, menuBackground.getSize().y));
            collapseMenu.move(40.0f, 0);
        }
        else if (menuBackground.getSize().x < MENU_WIDTH)
        {
            showMenuItems = true;
            collapseMenu.move(MENU_WIDTH - menuBackground.getSize().x, 0);
            menuBackground.setSize(Vector2f(MENU_WIDTH, menuBackground.getSize().y));
            collapseMenu.SetText("<");
        }
    }
    window.draw(menuBackground);
    window.draw(collapseMenu);

    if (showMenuItems)
    {
        constG.Update(mouseX, mouseY);
        stateBtn.Update(mouseX, mouseY);
        cb.Update(mouseX, mouseY);
        window.draw(menuTitle);
        window.draw(constG);
        window.draw(stateBtn);
        window.draw(cb);
    }
}

void StartPause()
{
    planetSystem.pause = !planetSystem.pause;
    stateBtn.SetText(planetSystem.pause ? "Start" : "Pause");
}

void CollapseMenu()
{
    isCollapsed = !isCollapsed;
}

void UpdateGConst(float value)
{
    planetSystem.SetGConst(value);
}

//Vertex line[] =
//{
//    Vertex(Vector2f(0, 0)),
//    Vertex(planet.getPosition())
//};
//line[1].color = Color::Black;
//
//target.draw(line, 2, Lines);
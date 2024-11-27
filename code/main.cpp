// File: main.cpp
#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"
#include <iostream>

int main() {
    auto mode = sf::VideoMode::getDesktopMode();
    int width = mode.width / 2, height = mode.height / 2;

    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set");
    ComplexPlane complexPlane(width, height);

    sf::Font font;
    font.loadFromFile("arial.ttf");  // Ensure arial.ttf is in your working directory.
    sf::Text text("", font, 14);
    text.setPosition(10, 10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left)
                    complexPlane.zoomIn();
                else if (event.mouseButton.button == sf::Mouse::Right)
                    complexPlane.zoomOut();
            } else if (event.type == sf::Event::MouseMoved) {
                complexPlane.setMouseLocation({event.mouseMove.x, event.mouseMove.y});
            }
        }
        complexPlane.updateRender();
        complexPlane.loadText(text);

        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}

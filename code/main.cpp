#include "ComplexPlane.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>     
#include <complex>   
#include <sstream>

using namespace sf;
using namespace std;

int main() {

	int width = VideoMode::getDesktopMode().width / 2;
	int height = VideoMode::getDesktopMode().height / 2;

	RenderWindow window(VideoMode(width, height), "Complex Plane", Style::Default);

	ComplexPlane complexPlane(width, height);

	sf::Font font;
	if (!font.loadFromFile("Arial.ttf")) {
		cout << "could not load font " << endl;
		return -1;
	}
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Green);
	text.setPosition(10, 10);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				// Quit the game when the window is closed
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
					complexPlane.zoomIn();
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
					complexPlane.zoomOut();
				}
			}
			if (event.type == sf::Event::MouseMoved) 
			{
				complexPlane.setMouseLocation({ event.mouseButton.x, event.mouseButton.y });
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				window.close();
			}
			else 
			{
				complexPlane.updateRender();
				complexPlane.loadText(text);

				window.clear();
				window.draw(complexPlane);
				window.draw(text);
				window.display();
			}
		}
	}
}


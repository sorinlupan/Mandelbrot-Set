#include "ComplexPlane.h"
#include <SFML/Audio.hpp>
#include <iostream> // debug

using namespace std;

int main()
{

    VideoMode vm(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);

    RenderWindow window(vm, "Mandelbrot!", Style::Default);
    ComplexPlane mandelbrot(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);

    sf::Font font;
    sf::Text text;
    text.setFont(font);
    if (!font.loadFromFile("arial.ttf"))
        throw runtime_error("Could not find font");

    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);



    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    //Zoom Out
                    //mandelbrot.zoomOut();
                    //setCenter
                    Vector2i zO = {event.mouseButton.x, event.mouseButton.y};
                    //cout << "DEBUG: zo (" << zO.x << ", " << zO.y << ")" << endl;
                    mandelbrot.setCenter(zO);
                    mandelbrot.zoomOut();
                }

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    //Zoom In
                    //mandelbrot.zoomIn();
                    //setCenter
                    Vector2i zI = {event.mouseButton.x, event.mouseButton.y};
                    //cout << "DEBUG: zi (" << zI.x << ", " << zI.y << ")" << endl;
                    mandelbrot.setCenter(zI);
                    mandelbrot.zoomIn();
                }
            }

            if (event.type == sf::Event::MouseMoved)
            {
                //setMouseLocation
                Vector2i mm = {sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y};
                //std::cout << "DEBUG: MM: (" << mm.x << ", " << mm.y << ")" << endl;
                mandelbrot.setMouseLocation(mm);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        //Update Scene
            //UpdateRender
            //LoadText

        mandelbrot.updateRender();
        mandelbrot.loadText(text);


        //Draw
            //Clear RenderWindow
            //ComplexPlane
            //Text
            //Display RenderWindow
        window.clear();
        //window.draw(mandelbrot);
        mandelbrot.draw(window, sf::RenderStates::Default);
        window.draw(text);
        window.display(); // ?
    }
    return 0;
}

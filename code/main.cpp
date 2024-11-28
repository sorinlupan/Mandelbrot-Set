#include "ComplexPlane.h"

using namespace std;

int main()
{

    VideoMode vm(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);

    RenderWindow window(vm, "Mandelbrot Set", Style::Default);
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

                    Vector2i zO = {event.mouseButton.x, event.mouseButton.y};

                    mandelbrot.setCenter(zO);
                    mandelbrot.zoomOut();
                }

                if (event.mouseButton.button == sf::Mouse::Left)
                {

                    Vector2i zI = {event.mouseButton.x, event.mouseButton.y};

                    mandelbrot.setCenter(zI);
                    mandelbrot.zoomIn();
                }
            }

            if (event.type == sf::Event::MouseMoved)
            {
                Vector2i mm = {sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y};
                mandelbrot.setMouseLocation(mm);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

 

        mandelbrot.updateRender();
        mandelbrot.loadText(text);

        window.clear();
        //window.draw(mandelbrot);
        mandelbrot.draw(window, sf::RenderStates::Default);
        window.draw(text);
        window.display();
    }
    return 0;
}

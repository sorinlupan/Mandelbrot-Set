#include "ComplexPlane.h"
#include <iostream>
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
    m_pixel_size = { pixelWidth, pixelHeight };
    m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;
    m_plane_center = { 0, 0 };
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_State = State::CALCULATING;
    m_vArray.setPrimitiveType(sf::Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray);
}
void ComplexPlane::updateRender() {
    if (m_State == State::CALCULATING) {
        
        m_vArray.resize(static_cast<size_t>(m_plane_size.x * m_plane_size.y));

        for (int i = 0; i < static_cast<int>(m_plane_size.y); ++i) {
            for (int j = 0; j < static_cast<int>(m_plane_size.x); ++j) {
                //calculate index
                size_t index = j + i * static_cast<size_t>(m_plane_size.x);

                //map pixel to complex coordinates
                sf::Vector2f coords = mapPixelToCoords({j, i});

                //calc mandelbrot iterations
                size_t iterations = countIterations(coords);

                //iterations to RGB
                sf::Uint8 r, g, b;
                iterationsToRGB(iterations, r, g, b);

                //set vertex position and color
                m_vArray[index].position = {static_cast<float>(j), static_cast<float>(i)};
                m_vArray[index].color = sf::Color(r, g, b);
            }
        }

        // Update state to displaying
        m_State = State::DISPLAYING;
    }
}


void ComplexPlane::zoomIn() {
    m_zoomCount++;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = {xSize, ySize};
    m_State = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    m_zoomCount--;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = {xSize, ySize};
    m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
    m_plane_center = mapPixelToCoords(mousePixel);
    m_State = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(sf::Text& text) {
    ostringstream ss;
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
    ss << "Mouse: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
    ss << "Left-click to zoom in" << endl;
    ss << "Right-click to zoom out" << endl;
    text.setString(ss.str());
}

sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i pixel) const {
   /*
    return {
        (pixel.x / m_plane_size.x) * m_plane_size.x + m_plane_center.x - m_plane_size.x / 2.0f,
        (pixel.y / m_plane_size.y) * m_plane_size.y + m_plane_center.y - m_plane_size.y / 2.0f
    };
    */
    float offsetX = m_plane_center.x - m_plane_size.x / 2.0f;
    float offsetY = m_plane_center.y - m_plane_size.y / 2.0f;
    float x = static_cast<float>(pixel.x) / m_pixel_size.x * m_plane_size.x,
    float y = static_cast<float>(pixel.y - m_pixel_size.y) / (0 - m_pixel_size.y) * m_plane_size.y;
    sf::Vector2f mapped = { (x + offsetX), (y + offsetY) };
    return mapped;
}

size_t ComplexPlane::countIterations(sf::Vector2f coord) const {
    complex<float> c(coord.x, coord.y), z(0, 0);
    size_t count = 0;
    while (abs(z) <= 4 && count < MAX_ITER) {
        z = z * z + c;
        count++;
    }
    return count;
}

void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const {
    if (count == MAX_ITER) { r = 0, g = 0, b = 0; }
    else if (count < 5) { r = 5, g = 5, b = 5;}
    else {
        float norm = static_cast<float>(count) / MAX_ITER;
        r = 255 * norm, g = 255 * norm, b = 255 * norm;
    }
}
#include "ComplexPlane.h"
#include <complex>  
using namespace sf;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) :
	m_pixel_size(pixelWidth, pixelHeight),
	m_aspectRatio(pixelWidth / pixelHeight),
	m_plane_center{ 0,0 }, m_plane_size{ BASE_WIDTH, BASE_HEIGHT * m_aspectRatio },
	m_zoomCount(0),
	m_state(State::CALCULATING),
	m_vArray(Points) {
	m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_vArray);
}
void ComplexPlane::zoomIn() {
	m_zoomCount++;
	int x = BASE_WIDTH * (pow(BASE_ZOOM,m_zoomCount));
	int y = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
	m_plane_size = { x, y };
	m_state = (State::CALCULATING);
}
void ComplexPlane::zoomOut() {
	m_zoomCount--;
	int x = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
	int y = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
	m_plane_size = { x, y };
	m_state = (State::CALCULATING);
}
void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
	m_plane_center = mapPixelToCoords(mousePixel);
	m_state = (State::CALCULATING);
}
void ComplexPlane::setMouseLocation(sf::Vector2i mousPixel) {
	m_mouseLocation = mapPixelToCoords(mousPixel);
}
void ComplexPlane::loadText(sf::Text& text) {
	stringstream s; 
}
void ComplexPlane::updateRender() {

}
int ComplexPlane::countIterations(sf::Vector2f coord) {

}
void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {

}
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {

}

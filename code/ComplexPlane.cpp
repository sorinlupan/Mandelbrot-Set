#include "ComplexPlane.h"
#include <iostream>
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
    m_pixel_size.x = pixelWidth;
    m_pixel_size.y = pixelHeight;
    //cout << "DEBUG: Constructor: pixel size x: " << m_pixel_size.x << "; y: " << m_pixel_size.y << endl;

    m_aspectRatio = (1.0*pixelHeight)/pixelWidth;

    m_plane_center = {0, 0};
    m_plane_size = {BASE_WIDTH, BASE_HEIGHT*m_aspectRatio};
    //cout << "DEBUG: Constructor: planesize: " << BASE_WIDTH << ", " << BASE_HEIGHT << "*" << m_aspectRatio << endl;
    m_zoomCount = 0;
    m_state = State::CALCULATING;

    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
    target.draw(m_vArray, states);
}

void ComplexPlane::updateRender() {
    //cout << "DEBUG: in updateRender" << endl;
    if (m_state == State::CALCULATING) {
        //cout << "DEBUG: UR: calculating" << endl;
        m_vArray.resize(m_pixel_size.x * m_pixel_size.y);
        for (int i=0; i<m_pixel_size.y; i++) {
            //cout << "DEBUG: UR: i = " << i << endl;
            for (int j=0; j< m_pixel_size.x; j++) {
                m_vArray[j+i*m_pixel_size.x].position = {(float)j, (float)i};
                //cout << "DEBUG: UR: vArray constructed" << endl;

                //mapPixelToCoords...
                Vector2i testI = {j, i};
                Vector2f testF;
                //cout << "DEBUG: UR: Vectors constructed" << endl;

                testF = mapPixelToCoords(testI);

                //countIterations..
                Uint8 r, g, b;
                size_t iters = countIterations(testF);
                ///iterationsToRGB...
                iterationsToRGB(iters, r, g, b);


                m_vArray[j+i*m_pixel_size.x].color = {r, g, b};
            }
        }
        //cout << "DEBUG: UR: made it out of loop!" << endl;
        m_state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn() {
    m_zoomCount++;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size.x = xSize;
    m_plane_size.y = ySize;
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    m_zoomCount--;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size.x = xSize;
    m_plane_size.y = ySize;
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) {
    //mapToCoords...
    Vector2f test = mapPixelToCoords(mousePixel);
    //cout << "DEBUG: SC..." << endl;
    //cout << "   Input: (" << mousePixel.x << ", " << mousePixel.y << ")" << endl;
    //cout << "   Coord: (" << test.x << ", " << test.y << ")" << endl;

    m_plane_center = test;
    //cout << "  Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")" << endl;
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel) {
    //mapToCoords...
    //cout << "DEBBUG: In setmouselocation" << endl;
    Vector2f test = mapPixelToCoords(mousePixel);

    m_mouseLocation = test;
    //cout << "DEBUG: SML: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << endl;
}

void ComplexPlane::loadText(Text& text) {
    //std::cout << "DEBUG: text loading" << endl;
    // stringstream
    std::stringstream testss;
    //std::string tests;
    testss << "Mandelbrot Set\n" <<
            "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n" <<
            "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n" <<
            "Left click to zoom in\n" <<
            "Right click to zoom out";
    //std::stringstream testss(tests);
    text.setString(testss.str());
}

size_t ComplexPlane::countIterations(Vector2f coord) {
    // uhh
    //z(i+1) = z(i)^2 + c; z(0) = 0 mandelbrot

    std::complex<float> c(coord.x, coord.y);
    std::complex<float> z = 0;
    int i=0;

    // mandelbrot
    
    while (std::norm(z) < 4.0 && i < MAX_ITER) {
        z = z*z + c;
        i++;
    }
    
    
    

    // burning ship
    /*
    c = -c;
    while (std::norm(z) < 4.0 && i < MAX_ITER) {
        std::complex<float> more(abs(z.real()), abs(z.imag()));
        z = more*more + c;
        i++;
    }
    */

    // messing around
    /*
    z = 0;
    while (std::norm(z) < 4.0 && i < MAX_ITER) {
        //std::complex<float> ma(z.real()+abs(z.imag()), z.imag());
        std::complex<float> ma(z.real()*z.real(), z.imag()*z.imag());
        std::complex<float> one(1.0, 0.0);
        z = z*z*z / (one+ma) + c;
        i++;
    }
    */


    return i;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {
    // oough.
    int range = MAX_ITER/11;
    // out -> in
    if (count < range) {
        r = 0;      g = 34;     b = 150;
    } else if (count < 2*range) {
        r = 44;     g = 23;     b = 148;
    } else if (count < 3*range) {
        r = 86;     g = 11;     b = 146;
    } else if (count < 4*range) {
        r = 129;    g = 0;      b = 144;
    } else if (count < 5*range) {
        r = 192;    g = 0;      b = 122;
    } else if (count < 6*range) {
        r = 234;    g = 12;     b = 95;
    } else if (count < 7*range) {
        r = 244;    g = 48;     b = 80;
    } else if (count < 8*range) {
        r = 254;    g = 83;     b = 65;
    } else if (count < 9*range) {
        r = 255;    g = 110;    b = 49;
    } else if (count < 10*range) {
        r = 255;    g = 136;    b = 32;
    } else {
        r = 251;    g = 161;    b = 17;
    }
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {
    // gug
    // okk sooo this maps... mousePixel to a Vector2f.... okkkkkkk
    // i think this is on click?
    // ((n  - a) / (b - a)) * (d - c) + c
    // sooooo
    // ughhhhh
    //cout << "DEBUG: In mapPixel" << endl;
    float debugX, debugY;
    // need to figure out how to get width and height of the screen

    /*
    debugX = ((mousePixel.x - 0) / (VideoMode::getDesktopMode().width - 0)) * (m_plane_size.x) +
             (m_plane_center.x - m_plane_size.x / 2.0);
    debugY = ((mousePixel.y - 0) / (VideoMode::getDesktopMode().height - 0)) * (m_plane_size.y) +
             (m_plane_center.y - m_plane_size.y / 2.0);
    */

    /*
    debugX = m_plane_center.x + ((float)mousePixel.x / m_pixel_size.x - 0.5) * m_plane_size.x;
    debugY = m_plane_center.y + ((float)mousePixel.y / (m_pixel_size.y - 0.5)) * m_plane_size.y;
    */

    
    float lowX = m_plane_center.x - m_plane_size.x / 2.0;
    float lowY = m_plane_center.y - m_plane_size.y / 2.0;

    debugX =  ((float)mousePixel.x / m_pixel_size.x)                        * (m_plane_size.x) + lowX;
    debugY = (((float)mousePixel.y - m_pixel_size.y) / (0-m_pixel_size.y))  * (m_plane_size.y) + lowY;
    //cout << "DEBUG: MP: " << (float)mousePixel.y - m_pixel_size.y << " / " << 0-m_pixel_size.y << endl;
    //cout << "* " << m_plane_size.y << " + " << lowY << endl;
    

    Vector2f ret = {debugX, debugY};
    //cout << "DEBUG: mapPixel ending" << endl;
    return ret;
}
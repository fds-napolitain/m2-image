//
//  Pixel.cpp
//  m2-image
//
//  Created by Maxime Boucher on 24/10/2021.
//

#include "Pixel.hpp"

PixelGray::PixelGray(unsigned char gray) {
    g = gray;
}

Pixel PixelGray::operator+(unsigned char value) {
    PixelGray pixel(g);
    pixel.g += value;
    return pixel;
}

Pixel PixelGray::operator-(unsigned char value) {
    PixelGray pixel(g);
    pixel.g -= value;
    return pixel;
}

Pixel PixelGray::operator*(unsigned char value) {
    PixelGray pixel(g);
    pixel.g *= value;
    return pixel;
}

Pixel PixelGray::operator/(unsigned char value) {
    PixelGray pixel(g);
    pixel.g /= value;
    return pixel;
}

Pixel PixelGray::operator+=(unsigned char value) {
    (*this).g += value;
    return *this;
}

Pixel PixelGray::operator-=(unsigned char value) {
    (*this).g -= value;
    return *this;
}

Pixel PixelGray::operator*=(unsigned char value) {
    (*this).g *= value;
    return *this;
}

Pixel PixelGray::operator/=(unsigned char value) {
    (*this).g /= value;
    return *this;
}

PixelRGB::PixelRGB(unsigned char red, unsigned char green, unsigned char blue) {
    r = red;
    g = green;
    b = blue;
}

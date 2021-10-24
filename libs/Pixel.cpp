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
    PixelGray pixel(g + value);
    return pixel;
}

Pixel PixelGray::operator-(unsigned char value) {
    PixelGray pixel(g - value);
    return pixel;
}

Pixel PixelGray::operator*(unsigned char value) {
    PixelGray pixel(g * value);
    return pixel;
}

Pixel PixelGray::operator/(unsigned char value) {
    PixelGray pixel(g / value);
    return pixel;
}

Pixel PixelGray::operator+=(unsigned char value) {
    g += value;
    return *this;
}

Pixel PixelGray::operator-=(unsigned char value) {
    g -= value;
    return *this;
}

Pixel PixelGray::operator*=(unsigned char value) {
    g *= value;
    return *this;
}

Pixel PixelGray::operator/=(unsigned char value) {
    g /= value;
    return *this;
}

Pixel PixelGray::operator++() {
    g++;
    return *this;
}

Pixel PixelGray::operator--() {
    g--;
    return *this;
}

Pixel PixelGray::operator&(unsigned char value) {
    PixelGray pixel(g & value);
    return pixel;
}

Pixel PixelGray::operator|(unsigned char value) {
    PixelGray pixel(g | value);
    return pixel;
}

Pixel PixelGray::operator&=(unsigned char value) {
    g &= value;
    return *this;
}

Pixel PixelGray::operator|=(unsigned char value) {
    g |= value;
    return *this;
}

Pixel PixelGray::operator^(unsigned char value) {
    PixelGray pixel(g ^ value);
    return pixel;}

Pixel PixelGray::operator^=(unsigned char value) {
    g ^= value;
    return *this;
}

Pixel PixelGray::operator~() {
    PixelGray pixel(~g);
    return pixel;
}

PixelRGB::PixelRGB(unsigned char red, unsigned char green, unsigned char blue) {
    r = red;
    g = green;
    b = blue;
}

Pixel PixelRGB::operator+(unsigned char value) {
    PixelRGB pixel(r + value, g + value, b + value);
    return pixel;
}

Pixel PixelRGB::operator-(unsigned char value) {
    PixelRGB pixel(r - value, g - value, b - value);
    return pixel;
}

Pixel PixelRGB::operator*(unsigned char value) {
    PixelRGB pixel(r * value, g * value, b * value);
    return pixel;
}

Pixel PixelRGB::operator/(unsigned char value) {
    PixelRGB pixel(r / value, g / value, b / value);
    return pixel;
}

Pixel PixelRGB::operator+=(unsigned char value) {
    r += value;
    g += value;
    b += value;
    return *this;
}

Pixel PixelRGB::operator-=(unsigned char value) {
    r -= value;
    g -= value;
    b -= value;
    return *this;
}

Pixel PixelRGB::operator*=(unsigned char value) {
    r *= value;
    g *= value;
    b *= value;
    return *this;
}

Pixel PixelRGB::operator/=(unsigned char value) {
    r /= value;
    g /= value;
    b /= value;
    return *this;
}

Pixel PixelRGB::operator++() {
    g++;
    return *this;
}

Pixel PixelRGB::operator--() {
    g--;
    return *this;
}

Pixel PixelRGB::operator&(unsigned char value) {
    PixelRGB pixel(r & value, g & value, b & value);
    return pixel;
}

Pixel PixelRGB::operator|(unsigned char value) {
    PixelRGB pixel(r | value, g | value, b | value);
    return pixel;
}

Pixel PixelRGB::operator&=(unsigned char value) {
    r &= value;
    g &= value;
    b &= value;
    return *this;
}

Pixel PixelRGB::operator|=(unsigned char value) {
    r |= value;
    g |= value;
    b |= value;
    return *this;
}

Pixel PixelRGB::operator^(unsigned char value) {
    PixelRGB pixel(r ^value, g ^ value, b ^ value);
    return pixel;}

Pixel PixelRGB::operator^=(unsigned char value) {
    r ^= value;
    g ^= value;
    b ^= value;
    return *this;
}

Pixel PixelRGB::operator~() {
    PixelRGB pixel(~r, ~g, ~b);
    return pixel;
}

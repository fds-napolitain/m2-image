//
//  Pixel.hpp
//  m2-image
//
//  Created by Maxime Boucher on 24/10/2021.
//

#ifndef Pixel_hpp
#define Pixel_hpp

#include <stdio.h>

/**
 * Classe représentant un pixel abstrait avec ses opérations
 * But: généricité par rapport aux composantes couleurs (niveaux de gris, rgb, etc).
 */
class Pixel {
    
public:
    Pixel();
    virtual ~Pixel();
    virtual Pixel operator+(unsigned char value);
    virtual Pixel operator-(unsigned char value);
    virtual Pixel operator*(unsigned char value);
    virtual Pixel operator/(unsigned char value);
    virtual Pixel operator+=(unsigned char value);
    virtual Pixel operator-=(unsigned char value);
    virtual Pixel operator*=(unsigned char value);
    virtual Pixel operator/=(unsigned char value);
    virtual Pixel operator++();
    virtual Pixel operator--();
    virtual Pixel operator&(unsigned char value);
    virtual Pixel operator|(unsigned char value);
    virtual Pixel operator&=(unsigned char value);
    virtual Pixel operator|=(unsigned char value);
    virtual Pixel operator^(unsigned char value);
    virtual Pixel operator^=(unsigned char value);
    virtual Pixel operator~();
};

/**
 * Classe représentant un pixel en niveau de gris
 */
class PixelGray : public Pixel {
    
public:
    unsigned char g;
    
    PixelGray(unsigned char gray);
    ~PixelGray();
    Pixel operator+(unsigned char value);
    Pixel operator-(unsigned char value);
    Pixel operator*(unsigned char value);
    Pixel operator/(unsigned char value);
    Pixel operator+=(unsigned char value);
    Pixel operator-=(unsigned char value);
    Pixel operator*=(unsigned char value);
    Pixel operator/=(unsigned char value);
    Pixel operator++();
    Pixel operator--();
    Pixel operator&(unsigned char value);
    Pixel operator|(unsigned char value);
    Pixel operator&=(unsigned char value);
    Pixel operator|=(unsigned char value);
    Pixel operator^(unsigned char value);
    Pixel operator^=(unsigned char value);
    Pixel operator~();
};

/**
 * Classe représentant un pixel en RGB
 */
class PixelRGB : public Pixel {
    
public:
    unsigned char r; // red
    unsigned char g; // green, or gray
    unsigned char b; // blue
    
    PixelRGB(unsigned char red, unsigned char green, unsigned char blue);
    ~PixelRGB();
    Pixel operator+(unsigned char value);
    Pixel operator-(unsigned char value);
    Pixel operator*(unsigned char value);
    Pixel operator/(unsigned char value);
    Pixel operator+=(unsigned char value);
    Pixel operator-=(unsigned char value);
    Pixel operator*=(unsigned char value);
    Pixel operator/=(unsigned char value);
    Pixel operator++();
    Pixel operator--();
    Pixel operator&(unsigned char value);
    Pixel operator|(unsigned char value);
    Pixel operator&=(unsigned char value);
    Pixel operator|=(unsigned char value);
    Pixel operator^(unsigned char value);
    Pixel operator^=(unsigned char value);
    Pixel operator~();
};

#endif /* Pixel_hpp */

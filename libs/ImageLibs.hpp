//
//  ImageLibs.hpp
//  m2-image
//
//  Created by Maxime Boucher on 11/10/2021.
//

#ifndef ImageLibs_hpp
#define ImageLibs_hpp

#include <stdio.h>
#include "ImageBase.hpp"

/**
 Structure d'un histogramme utilisé par ImageBase::histogram()
 */
struct Histogram {
    int data[256][3];
};

ImageBase fusion4(ImageBase i1, ImageBase i2, ImageBase i3, ImageBase i4);
ImageBase reconstructionHaar4(ImageBase lf, ImageBase mf_v, ImageBase mf_h, ImageBase hf, int q);

#endif /* ImageLibs_hpp */

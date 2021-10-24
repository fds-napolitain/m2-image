//
//  Histogram.hpp
//  m2-image
//
//  Created by Maxime Boucher on 24/10/2021.
//

#ifndef Histogram_hpp
#define Histogram_hpp

#include <stdio.h>

/**
 Structure d'un histogramme utilisé par ImageBase::histogram()
 */
class Histogram {
public:
    int data[256][3];
    
    
};

#endif /* Histogram_hpp */

//
//  main.cpp
//  tp4-ondelette
//
//  Created by Maxime Boucher on 04/10/2021.
//

#include <iostream>
#include <math.h>
#include "../libs/ImageBase.hpp"

int main(int argc, char * argv[]) {
    ImageBase imIn;
    if (argc > 1) {
        std::cout << argv[0];
        imIn.load(argv[1]);
    } else {
        std::cout << "Argument 1: chemin d'image\n";
        exit(1);
    }
    
    ImageBase imOut = imIn.ondelette_haar(1, 2, false);
    imOut.save("tp4-intermediaire.pgm");
    imOut = imIn.ondelette_haar(1, 2, true);
    imOut.save("tp4-compresse.pgm");
}

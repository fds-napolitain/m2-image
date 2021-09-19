//
//  main.cpp
//  tp2-espace-couleur
//
//  Created by Maxime Boucher on 18/09/2021.
//

#include "../libs/ImageBase.h"
#include <iostream>

ImageBase imIn;
ImageBase imOut;

int main(int argc, char **argv) {
    if (argc > 1) {
        std::cout << argv[0];
        imIn.load(argv[1]);
    } else {
        std::cout << "Argument 1: chemin d'image\n";
        exit(1);
    }
    imOut.shallowCopy(imIn);
    
    // réduire de moitié 2 composantes de RGB (ex: R et B)
    for (int y = 0; y < imIn.getHeight(); y++) {
        for (int x = 0; imIn.getWidth(); x++) {
            
        }
    }
    
    return 0;
}

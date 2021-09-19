//
//  main.cpp
//  tp2-espace-couleur
//
//  Created by Maxime Boucher on 18/09/2021.
//

#include "../libs/ImageBase.h"
#include <stdio.h>
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
    // réechantillonner pour avoir une taille originale de l'image
    for (int y = 0; y < imIn.getHeight(); y++) {
        for (int x = 0; imIn.getWidth(); x++) {
            std::cout << imOut[y*3][x*3];
            if (x % 2 == 0 && y % 2 == 0) {
                imOut[y*3][x*3] = imIn[y*3][x*3];
                imOut[y*3][x*3+2] = imIn[y*3][x*3+2];
            }
            imOut[y*3][x*3+1] = imIn[y*3][x*3+1];
        }
    }
    
    imOut.save("tp2-i1.ppm");
    
    return 0;
}

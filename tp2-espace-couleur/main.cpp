//
//  main.cpp
//  tp2-espace-couleur
//
//  Created by Maxime Boucher on 18/09/2021.
//

#include "../libs/ImageBase.h"
#include <stdio.h>
#include <iostream>

unsigned char moyenneCouleur(unsigned char a, unsigned char b) {
    return (unsigned char) ((int) a + (int) b) / 2;
}

unsigned char moyenneCouleur(unsigned char a, unsigned char b, unsigned char c) {
    return (unsigned char) ((int) a + (int) b + (int) c) / 3;
}

unsigned char moyenneCouleur(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
    return (unsigned char) ((int) a + (int) b + (int) c + (int) d) / 4;
}

int main(int argc, char **argv) {
    ImageBase imIn;
    if (argc > 1) {
        std::cout << argv[0];
        imIn.load(argv[1]);
    } else {
        std::cout << "Argument 1: chemin d'image\n";
        exit(1);
    }
    ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
    
    // réduire de moitié 2 composantes de RGB (ex: R et B)
    // réechantillonner pour avoir une taille originale de l'image
    for (int y = 0; y < imIn.getHeight(); y++) {
        for (int x = 0; x < imIn.getWidth(); x++) {
            if (x % 2 == 1 || y % 2 == 1) {
                if (x != imIn.getWidth()-1 && y != imIn.getHeight()-1) {
                    imOut[y*3][x*3] = (imIn[(y-1)*3][x*3] + imIn[(y+1)*3][x*3] + imIn[y*3][(x-1)*3] + imIn[(y+1)*3][(x+1)*3]) / 4;
                    imOut[y*3][x*3+2] = (imIn[y*3][(x-1)*3+2] + imIn[y*3][(x+1)*3+2] + imIn[(y-1)*3][x*3+2] + imIn[(y+1)*3][x*3+2]) / 4;
                }
                if (x == imIn.getWidth()-1 && y != imIn.getHeight()-1) {
                    
                }
            } else {
                imOut[y*3][x*3] = imIn[y*3][x*3];
                imOut[y*3][x*3+2] = imIn[y*3][x*3+2];
            }
            imOut[y*3][x*3+1] = imIn[y*3][x*3+1];
        }
    }
    
    imOut.save("tp2-i1.ppm");
}

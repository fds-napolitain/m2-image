//
//  main.cpp
//  tp3-huffman
//
//  Created by Maxime Boucher on 26/09/2021.
//

#include "../libs/ImageBase.hpp"
#include <stdio.h>
#include <iostream>

int main(int argc, char** argv) {
    ImageBase imIn;
    if (argc > 1) {
        std::cout << argv[0];
        imIn.load(argv[1]);
    } else {
        std::cout << "Argument 1: chemin d'image\n";
        exit(1);
    }
    ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
    
    // question 1: histogramme
    imIn.histogram();
}

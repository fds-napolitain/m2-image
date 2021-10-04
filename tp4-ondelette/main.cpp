//
//  main.cpp
//  tp4-ondelette
//
//  Created by Maxime Boucher on 04/10/2021.
//

#include <iostream>
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
    ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

}

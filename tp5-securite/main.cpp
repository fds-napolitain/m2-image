//
//  main.cpp
//  tp5-securite
//
//  Created by Maxime Boucher on 11/10/2021.
//

#include <iostream>
#include "ImageBase.hpp"

int main(int argc, char * argv[]) {
    ImageBase imIn;
    if (argc > 1) {
        std::cout << argv[0];
        imIn.load(argv[1]);
    } else {
        std::cout << "Argument 1: chemin d'image\n";
        exit(1);
    }
    
    ImageBase derived_key = imIn.derive_key(55);
    ImageBase imOut = imIn.get_xor(derived_key);
    imOut.save("tp5-i1-chiffre.pgm");
    imOut = imOut.get_xor(derived_key);
    imOut.save("tp5-i1-dechiffre.pgm");
}

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
    ImageBase chiffre1 = imIn.get_xor(derived_key);
    chiffre1.save("tp5-i1-chiffre-55.pgm");
    ImageBase dechiffre = chiffre1.get_xor(derived_key);
    dechiffre.save("tp5-i1-dechiffre-55.pgm");
    
    ImageBase derived_key2 = imIn.derive_key(18);
    ImageBase chiffre2 = imIn.get_xor(derived_key2);
    chiffre2.save("tp5-i1-chiffre-18.pgm");
    dechiffre = chiffre2.get_xor(derived_key);
    dechiffre.save("tp5-i1-dechiffre-18.pgm");
    
    // image différente (1.4)
    std::cout << "\n" << chiffre1.PSNR(chiffre2) << "\n";
    
    // 2.1
    std::cout << imIn.PSNR(chiffre1) << "\n";
    
    // 2.2
    std::cout << chiffre1.get_entropy(chiffre1.histogram());
}

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
        imIn.load(argv[1]);
    } else {
        std::cout << "Argument 1: chemin d'image\nArgument 2 optionnel: chemin d'image à insérer";
        exit(1);
    }
    std::cout << "\n";
    
    // questions 1...
    ImageBase derived_key = imIn.derive_key(55);
    ImageBase chiffre1;
//    chiffre1 = imIn.get_xor(derived_key);
//    chiffre1.save("tp5-chiffre-55.pgm");
//    ImageBase dechiffre = chiffre1.get_xor(derived_key);
//    dechiffre.save("tp5-dechiffre-55.pgm");
    
//    ImageBase derived_key2 = imIn.derive_key(18);
//    ImageBase chiffre2 = imIn.get_xor(derived_key2);
//    chiffre2.save("tp5-chiffre-18.pgm");
//    dechiffre = chiffre2.get_xor(derived_key);
//    dechiffre.save("tp5-dechiffre-18.pgm");
    
//    // image différente (1.4)
//    std::cout << "\nPSNR: " << chiffre1.PSNR(chiffre2) << "\n";
//
//    // 2.1
//    std::cout << "PSNR: " << imIn.PSNR(chiffre1) << "\n";
//
//    // 2.2
//    std::cout << "entropie: " << imIn.get_entropy(imIn.histogram());
//    std::cout << "entropie: " << chiffre1.get_entropy(chiffre1.histogram());
    
    // 2.3 a mettre en commentaire si jamais
    //imIn.histogram();
    
//    // 2.4
//    ImageBase plan;
//    plan = imIn.get_bit_plane(0, true);
//    plan.save("tp5-bit-plane-1.pgm");
//    plan = imIn.get_bit_plane(7, true);
//    plan.save("tp5-bit-plane-8.pgm");
//    plan = chiffre1.get_bit_plane(0, true);
//    plan.save("tp5-bit-plane-1.pgm");
//    plan = chiffre1.get_bit_plane(7, true);
//    plan.save("tp5-bit-plane-8.pgm");
    
    // 2.5
    // flux aléatoire
    ImageBase aInserer(imIn.getWidth()/2, imIn.getHeight()/4, imIn.getColor());
    if (argc > 2) { // image a inserer (argv[2]) (bonus)
        aInserer.load(argv[2]);
        aInserer = aInserer.get_xor(aInserer.derive_key(123)); // chiffre = xor(clé)
    } else { // image a inserer sinon : flux aléatoirement généré
        aInserer.random();
    }
    ImageBase insertion = imIn.insert_message(aInserer, 7);
//    insertion.save("tp5-image-insere-msb.pgm");
//    insertion.histogram();
//    std::cout << "PSNR: " << imIn.PSNR(insertion) << "\n";
//    insertion = imIn.insert_message(aInserer, 5);
//    std::cout << "PSNR: " << imIn.PSNR(insertion) << "\n";
//    insertion = imIn.insert_message(aInserer, 2);
//    std::cout << "PSNR: " << imIn.PSNR(insertion) << "\n";
//    insertion = imIn.insert_message(aInserer, 0);
//    std::cout << "PSNR: " << imIn.PSNR(insertion) << "\n";
//    insertion.save("tp5-image-insere-lsb.pgm");
//    insertion.histogram();
    
    // 3.1-5
    ImageBase pretraite = imIn.pretraitement(); // pretraitement
    chiffre1 = pretraite.get_xor(derived_key); // chiffrement
    insertion = chiffre1.insert_message(aInserer, 7, true); // insertion
    insertion = insertion.get_xor(derived_key); // dechiffrement
    std::cout << "PSNR: " << imIn.PSNR(insertion) << "\n";
    insertion.save("tp5-image-insere-chiffre.pgm");
    ImageBase reconstruct = insertion.reconstruct(); // reconstruction
    reconstruct.save("tp5-image-insere-reconstruct.pgm");
    std::cout << "PSNR: " << imIn.PSNR(reconstruct) << "\n";
}

//
//  main.cpp
//  tp5-securite
//
//  Created by Maxime Boucher on 11/10/2021.
//

#include <iostream>
#include "ImageBase.hpp"
#include "ImageLibs.hpp"

int main(int argc, char * argv[]) {
    ImageBase imIn;
    if (argc > 1) {
        imIn.load(argv[1]);
    } else {
        std::cout << "Argument 1: chemin d'image\nArgument 2 optionnel: chemin d'image à insérer";
        exit(1);
    }
    std::cout << "\n\n";
    
    // questions 1...
    ImageBase derived_key = imIn.derive_key(55);
    ImageBase chiffre1;
    chiffre1 = imIn ^ derived_key;
    chiffre1.save("tp5-chiffre-55.pgm");
    ImageBase dechiffre = chiffre1 ^ derived_key;
    dechiffre.save("tp5-dechiffre-55.pgm");
    
    ImageBase derived_key2 = imIn.derive_key(18);
    ImageBase chiffre2 = imIn ^ derived_key2;
    chiffre2.save("tp5-chiffre-18.pgm");
    dechiffre = chiffre2 ^ derived_key;
    dechiffre.save("tp5-dechiffre-18.pgm");
    
    // image différente (1.4)
    std::cout << "PSNR: " << chiffre1.PSNR(chiffre2) << "\n";

    // 2.1
    std::cout << "PSNR: " << imIn.PSNR(chiffre1) << "\n";

    // 2.2
    std::cout << "entropie: " << imIn.get_entropy(imIn.histogram()) << "\n";
    std::cout << "entropie: " << chiffre1.get_entropy(chiffre1.histogram()) << "\n";
    
    // 2.3 a mettre en commentaire si jamais
    imIn.histogram();
    
    // 2.4
    ImageBase plan;
    plan = imIn.get_bit_plane(0, true);
    plan.save("tp5-bit-plane-1.pgm");
    plan = imIn.get_bit_plane(7, true);
    plan.save("tp5-bit-plane-8.pgm");
    plan = chiffre1.get_bit_plane(0, true);
    plan.save("tp5-bit-plane-1.pgm");
    plan = chiffre1.get_bit_plane(7, true);
    plan.save("tp5-bit-plane-8.pgm");
    
    // 2.5
    // flux aléatoire
    ImageBase aInserer(imIn.getWidth()/2, imIn.getHeight()/4, imIn.getColor());
    if (argc > 2) { // 4.2 image a inserer (argv[2]), testé avec width/2 et height/4 de l'image cible
        aInserer.load(argv[2]);
        while (aInserer.getTotalSize() < imIn.getTotalSize() / 8) { // si image < 8 fois moins la taille de l'image
            ImageBase random(aInserer.getWidth(), aInserer.getHeight(), aInserer.getColor());
            random.random(); // remplir le reste d'aleatoire pour embeter une steganalyse éventuelle
            aInserer = fusion4(aInserer, random, random, random); // ^2 size filling with random bits
        }
        aInserer = aInserer ^ aInserer.derive_key(123); // chiffre = message xor clé
    } else { // image a inserer sinon : flux aléatoirement généré
        aInserer.random();
    }
    ImageBase insertion;
    insertion = imIn.insert_message(aInserer, 7);
    insertion.save("tp5-image-insere-msb.pgm");
    insertion.histogram();
    std::cout << "PSNR: " << imIn.PSNR(insertion) << "\n";
    insertion = imIn.insert_message(aInserer, 5);
    std::cout << "PSNR: " << imIn.PSNR(insertion) << "\n";
    insertion = imIn.insert_message(aInserer, 2);
    std::cout << "PSNR: " << imIn.PSNR(insertion) << "\n";
    insertion = imIn.insert_message(aInserer, 0);
    std::cout << "PSNR: " << imIn.PSNR(insertion) << "\n";
    insertion.save("tp5-image-insere-lsb.pgm");
    insertion.histogram();
    
    // 4.1
    ImageBase extract = insertion.extract_message(0);
    extract.save("tp5-image-insere-extract.pgm");
    
    // 3.1-5
    ImageBase pretraite = imIn.pretraitement(); // pretraitement
    chiffre1 = pretraite ^ derived_key; // chiffrement
    insertion = chiffre1.insert_message(aInserer, 7, true); // insertion
    insertion = insertion ^ derived_key; // dechiffrement
    std::cout << "PSNR: " << imIn.PSNR(insertion) << "\n";
    insertion.save("tp5-image-insere-chiffre.pgm");
    ImageBase reconstruct = insertion.reconstruct(); // reconstruction
    reconstruct.save("tp5-image-insere-reconstruct.pgm");
    std::cout << "PSNR: " << imIn.PSNR(reconstruct) << "\n";
}

//
//  ImageLibs.cpp
//  m2-image
//
//  Created by Maxime Boucher on 11/10/2021.
//

#include "ImageLibs.hpp"

/**
 * Assemble 4 images de taille égale tel que
 * i1 i2
 * i3 i4
 * =>
 * i5 i5
 * i5 i5
 */
ImageBase fusion4(ImageBase i1, ImageBase i2, ImageBase i3, ImageBase i4) {
    ImageBase imOut(i1.getWidth()*2, i1.getHeight()*2, i1.getColor());
    std::cout << "Fusion!\n";
    for (int j = 0; j < i1.getHeight(); j++) {
        for (int i = 0; i < i1.getWidth(); i++) {
            imOut[j][i] = i1[j][i];
            imOut[j][i1.getWidth() + i] = i2[j][i];
            imOut[i1.getHeight() + j][i] = i3[j][i];
            imOut[i1.getHeight() + j][i1.getWidth() + i] = i4[j][i];
        }
    }
    return imOut;
}

/**
 * Fonction utilisée dans la transformée en ondelette de Haar avec reconstruction.
 * Assemble 4 images de taille égale tel que
 * lf mf
 * mf hf
 * =>
 * LF LF
 * LF LF (upsampled)
 */
ImageBase reconstruction_haar_4(ImageBase lf, ImageBase mf_v, ImageBase mf_h, ImageBase hf, int q) {
    ImageBase imOut(lf.getWidth()*2, lf.getHeight()*2, lf.getColor());
    std::cout << "Reconstruction!\n";
    for (int j = 0; j < lf.getHeight(); j++) {
        for (int i = 0; i < lf.getWidth(); i++) {
            int v1 = (int)lf[j][i];
            int v2 = (int)(mf_v[j][i]-128)*2*q;
            int v3 = (int)(mf_h[j][i]-128)*2*q;
            int v4 = (int)(hf[j][i]-128)*2*q;
            imOut[j*2][i*2] = (char) std::clamp((v1 + v2 + v3 + v4), 0, 255);
            imOut[j*2+1][i*2] = (char) std::clamp((v1 + v2 - v3 - v4), 0, 255);
            imOut[j*2][i*2+1] = (char) std::clamp((v1 - v2 + v3 - v4), 0, 255);
            imOut[j*2+1][i*2+1] = (char) std::clamp((v1 - v2 - v3 + v4), 0, 255);
        }
    }
    return imOut;
}

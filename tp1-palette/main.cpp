#include "../libs/ImageBase.hpp"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>

typedef struct couleur {
    int r;
    int g;
    int b;
} couleur;

std::vector<couleur> palette;

struct {
    std::vector<couleur> palette;
    std::vector<int> n;
} paletteMoyenne;

ImageBase imIn;
ImageBase imOut;

void initPaletteRandom(int n) {
    palette.resize(n);
    paletteMoyenne.palette.resize(n);
    paletteMoyenne.n.resize(n);
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        // palette[i].r = rand() % 256;
        // palette[i].g = rand() % 256;
        // palette[i].b = rand() % 256;
        palette[i].r = imIn[rand() % imIn.getHeight()][rand() % imIn.getWidth()];
        palette[i].g = imIn[rand() % imIn.getHeight()][rand() % imIn.getWidth()+1];
        palette[i].b = imIn[rand() % imIn.getHeight()][rand() % imIn.getWidth()+2];
    }
}

// si palette et palette moyenne sont identiques (= convergent)
bool samePalette() {
    for (int i = 0; i < palette.size(); i++) {
        if (!(palette[i].r == paletteMoyenne.palette[i].r &&
        palette[i].g == paletteMoyenne.palette[i].g &&
        palette[i].b == paletteMoyenne.palette[i].b)) return false;
    }
    return true;
}

// kmean
float dist(int x, int y, int indicePalette) {
    return sqrt(
        pow(imIn[y*3][x*3] - palette[indicePalette].r, 2) +
        pow(imIn[y*3][x*3+1] - palette[indicePalette].g, 2) +
        pow(imIn[y*3][x*3+2] - palette[indicePalette].b, 2)
    );
}

// devrait etre bon
void parcoursKmean() {
    for(int y = 0; y < imIn.getHeight(); y++) {
        for (int x = 0; x < imIn.getWidth(); x++) {
            int indexPalette = 0;
            float distMin = INFINITY;
            for (int i = 0; i < palette.size(); i++) {
                float d = dist(x, y, i);
                if (d < distMin) {
                    distMin = d;
                    indexPalette = i;
                }
            }
            imOut[y*3][x*3] = palette[indexPalette].r;
            imOut[y*3][x*3+1] = palette[indexPalette].g;
            imOut[y*3][x*3+2] = palette[indexPalette].b;
            paletteMoyenne.palette[indexPalette].r += (int)imIn[y*3][x*3];
            paletteMoyenne.palette[indexPalette].g += (int)imIn[y*3][x*3+1];
            paletteMoyenne.palette[indexPalette].b += (int)imIn[y*3][x*3+2];
            paletteMoyenne.n[indexPalette]++;
        }
    }
    for (int i = 0; i < palette.size(); i++) {
        if (paletteMoyenne.n[i] == 0) continue;
        paletteMoyenne.palette[i].r /= paletteMoyenne.n[i];
        paletteMoyenne.palette[i].g /= paletteMoyenne.n[i];
        paletteMoyenne.palette[i].b /= paletteMoyenne.n[i];
    }
}

// bug ici de logique
void parcoursKmeanMoyenne() {
    int indexPalette = 0;
    float distMin = INFINITY;
    for(int y = 0; y < imIn.getHeight(); y++) {
        for (int x = 0; x < imIn.getWidth(); x++) {
            for (int i = 0; i < palette.size(); i++) {
                float d = dist(x, y, indexPalette);
                if (d < distMin) {
                    distMin = d;
                    indexPalette = i;
                    imOut[y*3][x*3] = paletteMoyenne.palette[indexPalette].r;
                    imOut[y*3][x*3+1] = paletteMoyenne.palette[indexPalette].g;
                    imOut[y*3][x*3+2] = paletteMoyenne.palette[indexPalette].b;
                }
            }
        }
    }
}

// dépend d'au dessus
void parcours8bits(ImageBase* imGrey) {
    for(int y = 0; y < imIn.getHeight(); y++) {
        for (int x = 0; x < imIn.getWidth(); x++) {
            int indexPalette = 0;
            float distMin = INFINITY;
            for (int i = 0; i < palette.size(); i++) {
                float d = dist(x, y, indexPalette);
                if (d < distMin) {
                    distMin = d;
                    indexPalette = i;
                }
            }
            (*imGrey)[y][x] = (unsigned char) indexPalette;
        }
    }
}

int main(int argc, char **argv) {
    if (argc > 1) {
        std::cout << argv[0];
        imIn.load(argv[1]);
    } else {
        std::cout << "Argument 1: chemin d'image\n";
        exit(1);
    }
    imOut.shallowCopy(imIn);

    // palette 256 couleurs
    couleur a = {0,0,0};
    palette.push_back(a);
    paletteMoyenne.palette.push_back(a);
    paletteMoyenne.n.push_back(0);
    initPaletteRandom(256);

    // image 1
    parcoursKmean();
    imOut.save("image-palette1.ppm");
    
    // image 2
    parcoursKmeanMoyenne();
    imOut.save("image-palette2.ppm");

    // image 3 (repeter)
    while (!samePalette()) {
        for (int i = 0; i < palette.size(); i++) {
            palette[i].r = paletteMoyenne.palette[i].r;
            palette[i].g = paletteMoyenne.palette[i].g;
            palette[i].b = paletteMoyenne.palette[i].b;
            paletteMoyenne.palette[i].r = 0;
            paletteMoyenne.palette[i].g = 0;
            paletteMoyenne.palette[i].b = 0;
            paletteMoyenne.n[i] = 0;
        }
        parcoursKmean();
        parcoursKmeanMoyenne();
    }
    imOut.save("image-palette3.ppm");

    // PSNR
    std::cout << imIn.PSNR(imOut);

    // 8bits
    ImageBase imGrey(imIn.getWidth(), imIn.getHeight(), false);
    parcours8bits(&imGrey);
    imGrey.save("image-palette4.pgm");
}

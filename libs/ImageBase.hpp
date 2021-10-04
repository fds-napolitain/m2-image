/******************************************************************************
* ICAR_Library
*
* Fichier : ImageBase.h
*
* Description : Classe contennant quelques fonctionnalit�s de base
*
* Auteur : Mickael Pinto
*
* Mail : mickael.pinto@live.fr
*
* Date : Octobre 2012
*
*******************************************************************************/

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>

#define ROUGE 0
#define VERT 1
#define BLEU 2


class ImageBase
{
	///////////// Enumerations
public:
	typedef enum { PLAN_R, PLAN_G, PLAN_B} PLAN;


	///////////// Attributs
protected:
	unsigned char *data;
	double *dataD;

	bool color;
	int height;
	int width;
	int nTaille;
	bool isValid;


	///////////// Constructeurs/Destructeurs
protected:
	void init();
	void reset();

public:
	ImageBase(void);
	ImageBase(int imWidth, int imHeight, bool isColor);
	~ImageBase(void);
	
	///////////// Methodes
protected:
	double EQM(ImageBase& imOut);

public:
	void copy(const ImageBase &copy);
	void shallowCopy(const ImageBase &copy);
	int getHeight() { return height; };
	int getWidth() { return width; };
	int getTotalSize() { return nTaille; };
	int getValidity() { return isValid; };
	bool getColor() { return color; };
	unsigned char *getData() { return data; };


	void load(char *filename);
	bool save(char *filename);

	ImageBase *getPlan(PLAN plan);

	unsigned char *operator[](int l);
	// Calcule le PSNR entre l'image actuel (imIn) et l'image cible (imOut).
	double PSNR(ImageBase& imOut);
    // Calcule la moyenne des couleurs de pixels voisins par coordonnées
    unsigned char average_color(int x, int y, std::vector<std::vector<int>> neighbors);
    ImageBase rgb_to_ycrcb();
    ImageBase ycrcb_to_rgb();
    void histogram();
    unsigned char difference(int x, int y);
    ImageBase ondelette_harr(int n);
    void operator+(const ImageBase& img);
};

ImageBase fusion4(ImageBase i1, ImageBase i2, ImageBase i3, ImageBase i4) {
    ImageBase imOut(i1.getWidth()*2, i1.getHeight()*2, i1.getColor());
    for (int j = 0; j < i1.getHeight(); j += 2) {
        for (int i = 0; i < i2.getWidth(); i += 2) {
            imOut[j][i] = i1[j][i];
            imOut[j][i1.getWidth() + i] = i2[j][i];
            imOut[i1.getHeight() + j][i] = i3[j][i];
            imOut[i1.getHeight() + j][i1.getWidth() + i] = i3[j][i];
        }
    }
    return imOut;
}

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
#include <vector>

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
};


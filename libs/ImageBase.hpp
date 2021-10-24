/******************************************************************************
* ICAR_Library
*
* Fichier : ImageBase.h
*
* Description : Classe contennant quelques fonctionnalit�s de base
*
* Auteur : Mickael Pinto, Maxime Boucher
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
#include "Histogram.hpp"
#include "Pixel.hpp"

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
	Pixel *data;

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
    /** Redéfinition de l'opérateur XOR entre 2 ImageBase */
    ImageBase operator^(ImageBase key);

	double PSNR(ImageBase& imOut);
    unsigned char average_color(int x, int y, std::vector<std::vector<int>> neighbors);
    ImageBase rgb_to_ycrcb();
    ImageBase ycrcb_to_rgb();
    Histogram histogram();
    unsigned char difference(int x, int y);
    ImageBase ondelette_haar(int n, int q, bool reconstruction);
    /** derive la clé de chiffrement initial sur une taille identique à l'image qu'on veut chiffrer (this) */
    ImageBase derive_key(unsigned int key);
    float get_entropy(Histogram histogram);
    ImageBase get_bit_plane(int k, bool binary);
    ImageBase insert_message(ImageBase img, int k, bool skip = false);
    unsigned char prediction(int x, int y);
    ImageBase reconstruct();
    void random();
    void zero();
    ImageBase pretraitement();
    ImageBase extract_message(int k, bool skip = false);
};

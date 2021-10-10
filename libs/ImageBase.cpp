/******************************************************************************
* ICAR_Library
*
* Fichier : ImageBase.cpp
*
* Description : Voir le fichier .h
*
* Auteur : Mickael Pinto
*
* Mail : mickael.pinto@live.fr
*
* Date : Octobre 2012
*
*******************************************************************************/

#include "ImageBase.hpp"
#include "image_ppm.hpp"

int clip(int n, int lower, int upper) {
    if (n < lower || n > upper) {
        return 255-n;
    }
    return n;
}

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
    std::cout << "\nFusion!";
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
 * i1 i2
 * i3 i4
 * =>
 * i5 i5
 * i5 i5
 */
ImageBase reconstructionHaar4(ImageBase i1, ImageBase i2, ImageBase i3, ImageBase i4) {
    ImageBase imOut(i1.getWidth()*2, i1.getHeight()*2, i1.getColor());
    std::cout << "\nReconstruction!";
    for (int j = 0; j < i1.getHeight(); j++) {
        for (int i = 0; i < i1.getWidth(); i++) {
            imOut[j*2][i*2] = i1[j][i];
            imOut[j*2][i*2+1] = i1[j][i] + i2[j][i] - 128;
            imOut[j*2+1][i*2] = i1[j][i] + i3[j][i] - 128;
            imOut[j*2][i*2+1] = i1[j][i] + i2[j][i] + i3[j][i] - 256;
        }
    }
    return imOut;
}

ImageBase::ImageBase(void)
{
	isValid = false;
	init();
}

ImageBase::ImageBase(int imWidth, int imHeight, bool isColor)
{
	isValid = false;
	init();

	color = isColor;
	height = imHeight;
	width = imWidth;
	nTaille = height * width * (color ? 3 : 1);
	
	if(nTaille == 0)
		return;
	
	allocation_tableau(data, OCTET, nTaille);
	dataD = (double*)malloc(sizeof(double) * nTaille);
	isValid = true;
}


ImageBase::~ImageBase(void)
{
	reset();
}

void ImageBase::init()
{
	if(isValid)
	{
		free(data);
		free(dataD);
	}

	data = 0;
	dataD = 0;
	height = width = nTaille = 0;
	isValid = false;
}

void ImageBase::reset()
{
	if(isValid)
	{
		//free(data);
		//free(dataD);
	}
	isValid = false;
}

void ImageBase::load(char *filename)
{
	init();

	int l = strlen(filename);

	if(l <= 4) // Le fichier ne peut pas etre que ".pgm" ou ".ppm"
	{
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut �tre que '.pgm' ou '.ppm'");
		exit(0);
	}

	int nbPixel = 0;

	if( strcmp(filename + l - 3, "pgm") == 0) // L'image est en niveau de gris
	{
		color = false;
		lire_nb_lignes_colonnes_image_pgm(filename, &height, &width);
		nbPixel = height * width;
  
		nTaille = nbPixel;
		allocation_tableau(data, OCTET, nTaille);
		lire_image_pgm(filename, data, nbPixel);
	}
	else if( strcmp(filename + l - 3, "ppm") == 0) // L'image est en couleur
	{
		color = true;
		lire_nb_lignes_colonnes_image_ppm(filename, &height, &width);
		nbPixel = height * width;
  
		nTaille = nbPixel * 3;
		allocation_tableau(data, OCTET, nTaille);
		lire_image_ppm(filename, data, nbPixel);
	}
	else 
	{
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut �tre que .pgm ou .ppm");
		exit(0);
	}
	
	dataD = (double*)malloc(sizeof(double) * nTaille);

	isValid = true;
}

bool ImageBase::save(char *filename)
{
	if(!isValid)
	{
		printf("Sauvegarde de l'image impossible : L'image courante n'est pas valide");
		exit(0);
	}

	if(color)
		ecrire_image_ppm(filename, data,  height, width);
	else
		ecrire_image_pgm(filename, data,  height, width);

	return true;
}

ImageBase *ImageBase::getPlan(PLAN plan)
{
	if( !isValid || !color )
		return 0;

	ImageBase *greyIm = new ImageBase(width, height, false);
	
	switch(plan)
	{
	case PLAN_R:
		planR(greyIm->data, data, height * width);
		break;
	case PLAN_G:
		planV(greyIm->data, data, height * width);
		break;
	case PLAN_B:
		planB(greyIm->data, data, height * width);
		break;
	default:
		printf("Il n'y a que 3 plans, les valeurs possibles ne sont donc que 'PLAN_R', 'PLAN_G', et 'PLAN_B'");
		exit(0);
		break;
	}

	return greyIm;
}

void ImageBase::copy(const ImageBase &copy)
{
	reset();
	
	isValid = false;
	init();
	
	color = copy.color;
	height = copy.height;
	width = copy.width;
	nTaille = copy.nTaille;
	isValid = copy.isValid;
	
	if(nTaille == 0)
		return;
	
	allocation_tableau(data, OCTET, nTaille);
	dataD = (double*)malloc(sizeof(double) * nTaille);
	isValid = true;

	for(int i = 0; i < nTaille; ++i)
	{
		data[i] = copy.data[i];
		dataD[i] = copy.dataD[i];
	}

}


void ImageBase::shallowCopy(const ImageBase &copy)
{
	reset();
	
	isValid = false;
	init();
	
	color = copy.color;
	height = copy.height;
	width = copy.width;
	nTaille = copy.nTaille;
	isValid = copy.isValid;
	
	if(nTaille == 0)
		return;
	
	allocation_tableau(data, OCTET, nTaille);
	dataD = (double*)malloc(sizeof(double) * nTaille);
	isValid = true;

}

unsigned char *ImageBase::operator[](int l)
{
	if(!isValid)
	{
		printf("L'image courante n'est pas valide");
		exit(0);
	}
	
	if((!color && l >= height) || (color && l >= height*3))
	{
		printf("L'indice se trouve en dehors des limites de l'image");
		exit(0);
	}
	
	return data+l*width;
}

double ImageBase::EQM(ImageBase& imOut) {
	double eqm_r = 0;
	double eqm_g = 0;
	double eqm_b = 0;

	for (int y = 0; y < this->getHeight(); y++) {
		for (int x = 0; x < this->getWidth(); x++) {
			eqm_r += pow((*this)[y][x] - imOut[y][x], 2);
			eqm_g += pow((*this)[y][x + 1] - imOut[y][x + 1], 2);
			eqm_b += pow((*this)[y][x + 2] - imOut[y][x + 2], 2);
		}
	}

	return (eqm_r + eqm_g + eqm_b) / (this->getTotalSize() * 3);
}

double ImageBase::PSNR(ImageBase& imOut) {
	return 10 * log10(pow(255, 2) / this->EQM(imOut));
}

unsigned char ImageBase::average_color(int x, int y, std::vector<std::vector<int>> neighbors) {
    int total = 0;
    int n = 0;
    for (int i = 0; i < neighbors.size(); i++) {
        if ((neighbors[i][0] >= 0 || neighbors[i][0] < (*this).getWidth()) &&
            (neighbors[i][1] >= 0 || neighbors[i][1] < (*this).getHeight())) {
            total += (int) (*this)[neighbors[i][1]][neighbors[i][0]];
            n++;
        }
    }
    return (unsigned char) total / n;
}

ImageBase ImageBase::rgb_to_ycrcb() {
    ImageBase imOut((*this).getWidth(), (*this).getHeight(), (*this).getColor());
    for (int y = 0; y < (*this).getHeight(); y++) {
        for (int x = 0; x < (*this).getWidth(); x++) {
            imOut[y*3][x*3] = 0.257 * (*this)[y*3][x*3] + 0.504 * (*this)[y*3][x*3+1] + 0.098* (*this)[y*3][x*3+2] + 16; // Y
            imOut[y*3][x*3+1] = -0.148 * (*this)[y*3][x*3] - 0.291 * (*this)[y*3][x*3+1] + 0.439 * (*this)[y*3][x*3+2] + 128; // Cr
            imOut[y*3][x*3+2] = 0.439 * (*this)[y*3][x*3] - 0.368 * (*this)[y*3][x*3+1] - 0.071 * (*this)[y*3][x*3+2] + 128; // Cb
        }
    }
    return imOut;
}

ImageBase ImageBase::ycrcb_to_rgb() {
    ImageBase imOut((*this).getWidth(), (*this).getHeight(), (*this).getColor());
    for (int y = 0; y < (*this).getHeight(); y++) {
        for (int x = 0; x < (*this).getWidth(); x++) {
            unsigned char cr = (int) imOut[y*3][x*3+1];
            unsigned char cb = (int) imOut[y*3][x*3+2];
            imOut[y*3][x*3] = (298.082 - (*this)[y*3][x*3]) / 256 + (408.583 - cr) / 256 - 222.921; // R
            imOut[y*3][x*3+1] = (298.082 - (*this)[y*3][x*3]) / 256 - (100.291 * cb) / 256 - (208.120 * cr) / 256 + 135.576; // g
            imOut[y*3][x*3+2] = (298.082 - (*this)[y*3][x*3]) / 256 + (516.412 * cb) / 256 - 276.836; // b
        }
    }
    return imOut;
}

/**
 Structure d'un histogramme utilisé par ImageBase::histogram()
 */
struct Histogram {
    int data[256][3];
};

/**
 Créé un histogramme histogram.csv à coté du binaire sous le format
 couleurs;rouge;vert;bleu
 */
void ImageBase::histogram() {
    std::ofstream outfile("histogram.csv");
    Histogram histogram;
    int depth = 256;
    if (color) { // rgb
        for (int i = 0; i < depth; i++) {
            histogram.data[i][0] = 0;
            histogram.data[i][1] = 0;
            histogram.data[i][2] = 0;
        }
        for (int i = 0; i < getHeight(); ++i) {
            for (int j = 0; j < getWidth(); ++j) {
                histogram.data[(*this)[i*3][j*3]][0]++;
                histogram.data[(*this)[i*3][j*3+1]][1]++;
                histogram.data[(*this)[i*3][j*3+2]][2]++;
            }
        }
        outfile << "couleurs;rouge;vert;bleu" << std::endl;
        for (int i = 0; i < depth; i++) {
            outfile << i << ";" << histogram.data[i][0] << ";" << histogram.data[i][1] << ";" << histogram.data[i][2] << std::endl;
        }
    } else { // greyscale
        for (int i = 0; i < depth; i++) {
            histogram.data[i][0] = 0;
        }
        for (int i = 0; i < getHeight(); ++i) {
            for (int j = 0; j < getWidth(); ++j) {
                histogram.data[(*this)[i][j]][0]++;
            }
        }
        outfile << "couleur;gris" << std::endl;
        for (int i = 0; i < depth; i++) {
            outfile << i << ";" << histogram.data[i][0] << std::endl;
        }
    }
    outfile.close();
    std::cout << "\nHistogramme écrit.\n";
}

/**
 Fonction en nuances de gris pour le moment
 */
unsigned char ImageBase::difference(int x, int y) {
    if (x == 0) {
        return (*this)[y][x];
    } else {
        return (*this)[y][x] - (*this)[y][x-1] + 128;
    }
}

/**
 * Fonctionne sur une image de taille binaire entière.
 * Retourne une image correspondant à la transformée en ondelette de Harr.
 * Si q = 1, alors pas de quantification.
 */
ImageBase ImageBase::ondelette_haar(int n, int q = 1, bool reconstruction = true) {
    ImageBase lower(getWidth()/2, getHeight()/2, getColor()); // low frequencies
    ImageBase medium_v(getWidth()/2, getHeight()/2, getColor());; // medium frequencies verticale
    ImageBase medium_h(getWidth()/2, getHeight()/2, getColor());; // medium frequencies horizontale
    ImageBase higher(getWidth()/2, getHeight()/2, getColor());; // higher frequencies
    if (!color) {
        for (int j = 0; j < getHeight()-1; j += 2) {
            for (int i = 0; i < getWidth()-1; i += 2) {
                lower[j/2][i/2] = ((*this)[j][i] + (*this)[j][i+1] + (*this)[j+1][i] + (*this)[j+1][i+1]) / 4;
                medium_v[j/2][i/2] = (((*this)[j][i] + (*this)[j][i+1] - (*this)[j+1][i] - (*this)[j+1][i+1]) / 2 + 256) / 511.0 * 255 / (q / 2);
                medium_h[j/2][i/2] = (((*this)[j][i] - (*this)[j][i+1] + (*this)[j+1][i] - (*this)[j+1][i+1]) / 2 + 256) / 511.0 * 255 / (q / 2);
                higher[j/2][i/2] = ((*this)[j][i] - (*this)[j][i+1] - (*this)[j+1][i] + (*this)[j+1][i+1] + 512) / 1023.0 * 255 / q;
            }
        }
    }
    std::cout << "\nOndelette!";
    if (!reconstruction) { // ondelette de haar aller simple
        if (n > 1) {
            if (q == 1) {
                return fusion4(lower.ondelette_haar(n-1, 1), medium_v, medium_h, higher);
            } else {
                return fusion4(lower.ondelette_haar(n-1, q / 2), medium_v, medium_h, higher);
            }
        } else {
            return fusion4(lower, medium_v, medium_h, higher);
        }
    } else { // ondelette de haar aller retour
        if (n > 1) {
            if (q == 1) {
                return reconstructionHaar4(lower.ondelette_haar(n-1, 1), medium_v, medium_h, higher);
            } else {
                return reconstructionHaar4(lower.ondelette_haar(n-1, q / 2), medium_v, medium_h, higher);
            }
        } else {
            return reconstructionHaar4(lower, medium_v, medium_h, higher);
        }
    }
}

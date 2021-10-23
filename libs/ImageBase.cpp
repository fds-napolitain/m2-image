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

#include <algorithm>
#include "ImageBase.hpp"
#include "image_ppm.hpp"
#include "ImageLibs.hpp"

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

    std::cout << "Image enregistrée.\n";
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
    if (getColor() == false) {
        double eqm = 0;

        for (int y = 0; y < getHeight(); y++) {
            for (int x = 0; x < getWidth(); x++) {
                eqm += pow((*this)[y][x] - imOut[y][x], 2);
            }
        }

        return eqm / (getTotalSize() * 3);
    } else {
        double eqm_r = 0;
        double eqm_g = 0;
        double eqm_b = 0;

        for (int y = 0; y < getHeight(); y++) {
            for (int x = 0; x < getWidth(); x++) {
                eqm_r += pow((*this)[y][x] - imOut[y][x], 2);
                eqm_g += pow((*this)[y][x + 1] - imOut[y][x + 1], 2);
                eqm_b += pow((*this)[y][x + 2] - imOut[y][x + 2], 2);
            }
        }

        return (eqm_r + eqm_g + eqm_b) / (getTotalSize() * 3);
    }
}

double ImageBase::PSNR(ImageBase& imOut) {
	return 10 * log10(pow(255, 2) / EQM(imOut));
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
 Créé un histogramme histogram.csv à coté du binaire sous le format
 couleurs;rouge;vert;bleu
 */
Histogram ImageBase::histogram() {
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
    std::cout << "Histogramme écrit.\n";
    return histogram;
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
 * Si q = 2, alors pas de quantification.
 */
ImageBase ImageBase::ondelette_haar(int n, int q = 2, bool reconstruction = true) {
    ImageBase lower(getWidth()/2, getHeight()/2, getColor()); // low frequencies
    ImageBase medium_v(getWidth()/2, getHeight()/2, getColor());; // medium frequencies verticale
    ImageBase medium_h(getWidth()/2, getHeight()/2, getColor());; // medium frequencies horizontale
    ImageBase higher(getWidth()/2, getHeight()/2, getColor());; // higher frequencies
    if (!color) {
        for (int j = 0; j < getHeight()-1; j += 2) {
            for (int i = 0; i < getWidth()-1; i += 2) {
                lower[j/2][i/2] = std::clamp(((*this)[j][i] + (*this)[j][i+1] + (*this)[j+1][i] + (*this)[j+1][i+1]) / 4, 0, 255);
                medium_v[j/2][i/2] = std::clamp(((*this)[j][i] + (*this)[j][i+1] - (*this)[j+1][i] - (*this)[j+1][i+1]) / 2 / (q * 2) + 128, 0, 255);
                medium_h[j/2][i/2] = std::clamp(((*this)[j][i] - (*this)[j][i+1] + (*this)[j+1][i] - (*this)[j+1][i+1]) / 2 / (q * 2) + 128, 0, 255);
                higher[j/2][i/2] = std::clamp(((*this)[j][i] - (*this)[j][i+1] - (*this)[j+1][i] + (*this)[j+1][i+1]) / (q * 2) + 128, 0, 255);
            }
        }
    }
    std::cout << "\nOndelette!";
    if (!reconstruction) { // ondelette de haar aller simple
        if (n > 1) {
            if (q == 2) {
                return fusion4(lower.ondelette_haar(n-1, 2, reconstruction), medium_v, medium_h, higher);
            } else {
                return fusion4(lower.ondelette_haar(n-1, q, reconstruction), medium_v, medium_h, higher);
            }
        } else {
            return fusion4(lower, medium_v, medium_h, higher);
        }
    } else { // ondelette de haar aller retour
        if (n > 1) {
            if (q == 2) {
                return reconstruction_haar_4(lower.ondelette_haar(n-1, 2), medium_v, medium_h, higher, q);
            } else {
                return reconstruction_haar_4(lower.ondelette_haar(n-1, q), medium_v, medium_h, higher, q);
            }
        } else {
            return reconstruction_haar_4(lower, medium_v, medium_h, higher, q);
        }
    }
}

ImageBase ImageBase::derive_key(unsigned int key) {
    ImageBase derived_key(getWidth(), getHeight(), getColor());
    srand(key);
    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth(); x++) {
            derived_key[y][x] = (char) rand() % 256;
        }
    }
    std::cout << "Clé dérivée.\n";
    return derived_key;
}

ImageBase ImageBase::get_xor(ImageBase key) {
    ImageBase imOut(getWidth(), getHeight(), getColor());
    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth(); x++) {
            imOut[y][x] = (*this)[y][x] ^ key[y][x];
        }
    }
    std::cout << "Image chiffrée avec XOR\n";
    return imOut;
}

float ImageBase::get_entropy(Histogram histogram) {
    float somme = 0;
    for (int i = 0; i < 256; i++) {
        float pi = histogram.data[i][0] / (float) getTotalSize();
        if (pi != 0)
            somme -= pi*(log(pi)/log(2));
    }
    return somme;
}

/**
 * Renvoit un masque pour plan binaire (avec un seul bit à 1)
 * k = 1 => LSB
 */
unsigned char get_bit_mask(int k) {
    return (unsigned char) pow(2, k);
}

/**
 * Inverse le MSB du nombre
 */
unsigned char inverse_msb(unsigned char pixel) {
    unsigned char result = pixel;
    if (pixel >= get_bit_mask(7)) { // msb == 1
        result &= ~get_bit_mask(7);
    } else { // msb == 0
        result |= get_bit_mask(7);
    }
    return result;
}

/* Exemple de fonctionnement du plan binaire
 *   011000000 11000000
 * & 001000000 00100000
 * = 001000000 00000000
 * Chaque pixel est ensuite enregistré dans l'image bit_plane
 * L'option binary permet d'afficher en noir et blanc plutôt que les valeurs en niveaux de gris correspondant au plan binaire (rapidement trop foncé).
 */
ImageBase ImageBase::get_bit_plane(int k, bool binary = false) {
    unsigned char mask = get_bit_mask(k);
    ImageBase bit_plane(getWidth(), getHeight(), getColor());
    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth(); x++) {
            if (binary) bit_plane[y][x] = ((*this)[y][x] & mask) >= mask ? 255 : 0;
            else bit_plane[y][x] = (*this)[y][x] & mask;
        }
    }
    return bit_plane;
}

/**
 * in place modification of image to be random
 */
void ImageBase::random() {
    srand(time(NULL));
    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth(); x++) {
            (*this)[y][x] = (unsigned char) rand() % 256;
        }
    }
}

/**
 * Predict pixel(x, y) using (x-1 y), (x-1 y-1), (x y-1).
 * Input has to be x and y > 0 (first line and column should always be skipped anyway for prediction to work).
 */
unsigned char ImageBase::prediction(int x, int y) {
    return (unsigned char) ((int)(*this)[y][x-1] + (int)(*this)[y-1][x-1] + (int)(*this)[y-1][x]) / 3;
}

/**
 * 1. Create output image of same size of original
 *   - Height and width factor are used to associate multiple pixels of a smaller image to only one of a bigger one (for iterating)
 * 2. Original mask : 11001100 & 11110111 => 11000100 (insert at k = 4)
 * 3. Insertion mask : list of all pow(2, 0...7) to select the bit of the message to insert 11010101 & 10000000 => 10000000
 * 4. Insert message :
 */
ImageBase ImageBase::insert_message(ImageBase img, int k, bool skip) {
    // initialisations
    ImageBase imOut(getWidth(), getHeight(), getColor());
    int height_factor = getHeight() / img.getHeight();
    int width_factor = getWidth() / img.getWidth();
    // mask for first image
    unsigned char mask1 = ~get_bit_mask(k); // k = 0 => 11111110
    // mask for second image
    std::vector<unsigned char> mask2;
    for (int i = 0; i < 8; i++) {
        mask2.push_back(get_bit_mask(i));
    }
    // inserting image into original image
    int i = 0;
    for (int y = 0; y < getHeight(); y++) {
        if (skip && (y == 0)) continue; // skip first column
        for (int x = 0; x < getWidth(); x++) {
            if (skip && (x == 0)) continue; // skip first line
            imOut[y][x] = (unsigned char) (((*this)[y][x] & mask1) | (((img[y/height_factor][x/width_factor] & mask2[i]) >> i) << k)); // insert message
            i = (i + 1) % 8;
        }
    }
    std::cout << "Image secrète insérée dans une image visible.\n";
    return imOut;
}

/**
 * this represents the image to be reconstructed, so the one which is encryped and with inserted data.
 */
ImageBase ImageBase::reconstruct() {
    // initialisations
    ImageBase imOut(getWidth(), getHeight(), getColor());
    for (int i = 0; i < imOut.getHeight(); i++) imOut[i][0] = (*this)[i][0];
    for (int i = 0; i < imOut.getWidth(); i++) imOut[0][i] = (*this)[0][i];
    // interpollate data
    for (int y = 1; y < getHeight(); y++) {
        for (int x = 1; x < getWidth(); x++) {
            if (abs((int)imOut.prediction(x, y) - (int)(*this)[y][x]) > abs((int)imOut.prediction(x, y) - (int)inverse_msb((*this)[y][x]))) { // if msb is inverted with neighbors
                imOut[y][x] = inverse_msb((*this)[y][x]); // inversion du msb de l'image
            } else {
                imOut[y][x] = (*this)[y][x];
            }
        }
    }
    std::cout << "Reconstruction de l'image par interpollation des MSB\n";
    return imOut;
}

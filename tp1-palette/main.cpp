#include "ImageBase.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

// couleur choisies
int c1 = 0;
int c2 = 255;

// couleurs moyennes calculées des classes
int c3 = 0;
int n3 = 0;
int c4 = 0;
int n4 = 0;

int d1(unsigned char pixel) {
    return sqrt((pixel - c1) + (pixel - c1) + (pixel - c1));
}

int d2(unsigned char pixel) {
    return sqrt((pixel - c2) + (pixel - c2) + (pixel - c2));
}

int main(int argc, char **argv) {
    ImageBase imIn;
    imIn.load("lena.ppm");

    ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
    ImageBase imOut2(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

    for(int x = 0; x < imIn.getTotalSize(); x += 3) {
        // classification avec kmean / distance
        if (d1(*imIn[x]) < d2(*imIn[x])) {
            *imOut[x] = c1;
            *imOut[x+1] = c1;
            *imOut[x+2] = c1;
            c3 += *imIn[x] + *imIn[x+1] + *imIn[x+2]; // on va dire ca our le moment
            n3++;
        } else {
            *imOut[x] = c2;
            *imOut[x+1] = c2;
            *imOut[x+2] = c2;
            c4 += *imIn[x] + *imIn[x+1] + *imIn[x+2]; // on va dire ca our le moment
            n4++;
        }
    }

    for(int x = 0; x < imIn.getTotalSize(); x += 3) {
        // classification avec kmean / distance
        if (d1(*imIn[x]) < d2(*imIn[x])) {
            *imOut2[x] = c3;
            *imOut2[x+1] = c3;
            *imOut2[x+2] = c3;
        } else {
            *imOut2[x] = c4;
            *imOut2[x+1] = c4;
            *imOut2[x+2] = c4;
        }
    }

    // repeter ici en changeant c1 c2 par c3 et c4 trouvées, jusqu'a que c3 ~= c4
    // peut etre manuellement

    imOut.save("lena_palette1.ppm");
    imOut2.save("lena_palette2.ppm");

}

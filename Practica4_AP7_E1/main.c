#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

void copiarBloque(int filaO, int colO, IplImage* imgO, int filaD, int colD, IplImage* imgD, int alto, int ancho) {
    int limFilaO, limColO;
    int tamBloque = 64;
    int colOCP = colO;
    int colDCP = colD;
    limFilaO = filaO + tamBloque;
    limColO = colO + tamBloque;
    for (filaO; filaO < limFilaO; filaO++) {
        unsigned char *pImag1 = (unsigned char *) (imgO->imageData + filaO * imgO->widthStep);
        unsigned char *pImag2 = (unsigned char *) (imgD->imageData + filaD * imgD->widthStep);
        colO = colOCP;
        colD = colDCP;
        pImag1 += colO*3;
        pImag2 += colD*3;
        for (colO; colO < limColO; colO++) {
            *pImag2++ = *pImag1++;
            *pImag2++ = *pImag1++;
            *pImag2++ = *pImag1++;
            colD++;
        }
        filaD++;
    }
}

int main(int argc, char** argv) {
    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    // Always check if the program can find a file
    if (!Img1) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }
    IplImage* Img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    // Always check if the program can find a file
    if (!Img2) {
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }
    cvShowImage(argv[1], Img1);
    cvShowImage(argv[2], Img2);
    cvWaitKey(0);
    int fila, columna;
    for(fila=0; fila<Img1->height; fila+=64){
        for(columna=0; columna<Img1->width; columna+=64){
            copiarBloque(fila, columna, Img1, fila, columna, Img2, Img1->height, Img1->width);
            cvWaitKey(0);
            cvShowImage(argv[2], Img2);
        }
    }
    //copiarBloque(600, 600, Img1, 600, 600, Img2, 768, 1024);
    cvShowImage(argv[2], Img2);
    cvWaitKey(0);
    return (EXIT_SUCCESS);
}

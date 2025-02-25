#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    IplImage* ImgGRIS = cvCreateImage(cvSize(256, 256), IPL_DEPTH_8U, 1);

    int fila, columna;
    
    for (fila = 0; fila < 256; fila++) {

        unsigned char *pImgGRIS = (unsigned char *) ImgGRIS->imageData + fila * ImgGRIS->widthStep;

        for (columna = 0; columna < 256; columna++) {
            *pImgGRIS = fila;
            *pImgGRIS++;
        }
    }  

    // crea y muestras las ventanas con las im genes
    cvNamedWindow("Gris", CV_WINDOW_AUTOSIZE);
    cvShowImage("Gris", ImgGRIS);

    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&ImgGRIS);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Gris");


    return EXIT_SUCCESS;
}
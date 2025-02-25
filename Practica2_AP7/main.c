#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* Img = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);

    if (!Img) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE);
    cvShowImage(argv[1], Img);
    cvWaitKey(0);
    cvNamedWindow("Img FUNDIDA", CV_WINDOW_AUTOSIZE);

    int fila, columna;

    int ciclo;

    for (ciclo = 0; ciclo < 256; ciclo++) {

        IplImage* ImgFUNDIDO = cvCreateImage(cvSize(Img->width, Img->height), IPL_DEPTH_8U, 3);

        for (fila = 0; fila < Img->height; fila++) {

            unsigned char *pImg = (unsigned char *) Img->imageData + fila * Img->widthStep;
            unsigned char *pImgFUNDIDO = (unsigned char *) ImgFUNDIDO->imageData + fila * ImgFUNDIDO->widthStep;

            for (columna = 0; columna < Img->width; columna++) {

                int canal;

                for (canal = 0; canal < 3; canal++) {
                    if (pImgFUNDIDO > *pImg - ciclo) {
                        *pImgFUNDIDO = *pImg - ciclo;
                    } else{
                        *pImgFUNDIDO = 0;
                    }
                    *pImgFUNDIDO++;
                    *pImg++;
                }
            }
        }


        cvShowImage("Img FUNDIDA", ImgFUNDIDO);
        cvWaitKey(1);

    }

    cvWaitKey(0);

    cvReleaseImage(&Img);
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Img FUNDIDA");

    return EXIT_SUCCESS;
}
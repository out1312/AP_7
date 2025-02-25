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

    const int pixelesExtra = 20;

    IplImage* ImgBORDES = cvCreateImage(cvSize(Img->width + pixelesExtra * 2, Img->height + pixelesExtra * 2), IPL_DEPTH_8U, 3);

    int fila, columna;

    for (fila = 0; fila < ImgBORDES->height; fila++) {

        unsigned char *pImg = (unsigned char *) Img->imageData + fila * Img->widthStep;
        unsigned char *pImgBORDES = (unsigned char *) ImgBORDES->imageData + fila * ImgBORDES->widthStep;

        for (columna = 0; columna < ImgBORDES->width; columna++) {

            if (columna <= pixelesExtra || columna >= ImgBORDES->height - pixelesExtra || fila <= pixelesExtra || fila >= ImgBORDES->width - pixelesExtra) {
                *pImgBORDES++ = 0;
                *pImgBORDES++ = 0;
                *pImgBORDES++ = 0;
            } else {
                *pImgBORDES++ = *pImg++;
                *pImgBORDES++ = *pImg++;
                *pImgBORDES++ = *pImg++;
            }
        }
    }

    // crea y muestras las ventanas con las im genes
    cvNamedWindow("BORDES", CV_WINDOW_AUTOSIZE);
    cvShowImage("BORDES", ImgBORDES);

    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&Img);
    cvReleaseImage(&ImgBORDES);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("BORDES");

    return EXIT_SUCCESS;
}
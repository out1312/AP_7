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

    IplImage* ImgCOMPUESTA = cvCreateImage(cvSize(Img->width, Img->height * 4), IPL_DEPTH_8U, 3);

    int fila, columna;

    int canal;

    for (canal = 0; canal < 4; canal++) {

        for (fila = 0; fila < Img->height; fila++) {

            unsigned char *pImg = (unsigned char *) Img->imageData + fila * Img->widthStep;
            unsigned char *pImgCOMPUESTA = (unsigned char *) ImgCOMPUESTA->imageData + fila * ImgCOMPUESTA->widthStep;
            
            pImgCOMPUESTA += (Img->widthStep * Img->height * canal);

            int tam = Img->height * Img->widthStep;

            for (columna = 0; columna < Img->width; columna++) {

                switch (canal) {
                    case 0:
                        *pImgCOMPUESTA++ = *pImg++;
                        *pImgCOMPUESTA++ = *pImg++;
                        *pImgCOMPUESTA++ = *pImg++;
                        break;
                    case 1:
                        *pImgCOMPUESTA++ = 0;
                        pImg++;
                        *pImgCOMPUESTA++ = 0;
                        pImg++;
                        *pImgCOMPUESTA++ = *pImg++;
                        break;
                    case 2:
                        *pImgCOMPUESTA++ = 0;
                        pImg++;
                        *pImgCOMPUESTA++ = *pImg++;
                        *pImgCOMPUESTA++ = 0;
                        pImg++;
                        break;
                    case 3:
                        *pImgCOMPUESTA++ = *pImg++;
                        *pImgCOMPUESTA++ = 0;
                        pImg++;
                        *pImgCOMPUESTA++ = 0;
                        pImg++;
                        break;
                }
            }
        }
    }
    // crea y muestras las ventanas con las im genes
    cvNamedWindow("Componente COMPUESTA", CV_WINDOW_AUTOSIZE);
    cvShowImage("Componente COMPUESTA", ImgCOMPUESTA);

    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&Img);
    cvReleaseImage(&ImgCOMPUESTA);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente COMPUESTA");


    return EXIT_SUCCESS;
}
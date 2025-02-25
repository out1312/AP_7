#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* ImgOrigen = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
    IplImage* ImgDestino = cvCreateImage(cvSize(ImgOrigen->width, ImgOrigen->height), IPL_DEPTH_8U, 3);

    // Always check if the program can find the image file
    if (!ImgOrigen) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    // a visualization window is created with title: image file name
    cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Img FUNDIDA", CV_WINDOW_AUTOSIZE);
    // Img is shown in 'image' window
    cvShowImage(argv[1], ImgOrigen);

    int fila, cc, ciclo;
    for (ciclo = 0; ciclo < 255; ciclo++) {
        for (fila = 0; fila < ImgOrigen->height; fila++) {
            __m128i *pImgOrigen = (__m128i *) (ImgOrigen->imageData + fila * ImgOrigen->widthStep);
            __m128i *pImgDestino = (__m128i *) (ImgDestino->imageData + fila * ImgDestino->widthStep);
            __m128i registro;
            for (cc = 0; cc < ImgOrigen->widthStep; cc = cc + 16) {
                //Opcion 1
                *pImgDestino = *pImgOrigen;
                __m128i B = _mm_set1_epi8(ciclo);
                *pImgDestino = _mm_subs_epu8(*pImgDestino, B);
                *pImgDestino++;
                *pImgOrigen++;
                //Opcion 2
                //registro = *pImgOrigen++;
                //*pImgDestino++ = registro;
                //Opcion 3
                //registro = _mm_load_si128(pImgOrigen++);
                //_mm_store_si128(pImgDestino++,registro);
                //Opcion 4, para datos no alineados en memoria
                //registro = _mm_loadu_si128(pImgOrigen++);
                //_mm_storeu_si128(pImgDestino++,registro);
            }
        }
        cvShowImage("Img FUNDIDA", ImgDestino);
        cvWaitKey(1);
    }


    // crea y muestras las ventanas con las im genes

    cvWaitKey(0);

    // memory release for images before exiting the application
    cvReleaseImage(&ImgOrigen);
    cvReleaseImage(&ImgDestino);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente BLUE");
    cvDestroyWindow("Componente GREEN");
    cvDestroyWindow("Componente RED");

    return EXIT_SUCCESS;
}
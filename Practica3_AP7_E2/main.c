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

    IplImage* ImgOrigen = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* ImgRojo = cvCreateImage(cvSize(ImgOrigen->width, ImgOrigen->height), IPL_DEPTH_8U, 4);
    IplImage* ImgVerde = cvCreateImage(cvSize(ImgOrigen->width, ImgOrigen->height), IPL_DEPTH_8U, 4);
    IplImage* ImgAzul = cvCreateImage(cvSize(ImgOrigen->width, ImgOrigen->height), IPL_DEPTH_8U, 4);

    cvShowImage("Img ORIGEN", ImgOrigen);
    cvWaitKey(0);

    __m128i azul = _mm_set1_epi32(0x000000FF);
    __m128i verde = _mm_set1_epi32(0x0000FF00);
    __m128i rojo = _mm_set1_epi32(0x00FF0000);

    int fila, cc;
    for (fila = 0; fila < ImgOrigen->height; fila++) {
        __m128i *pImgOrigen = (__m128i *) (ImgOrigen->imageData + fila * ImgOrigen->widthStep);
        __m128i *pImgRojo = (__m128i *) (ImgRojo->imageData + fila * ImgRojo->widthStep);
        __m128i *pImgVerde = (__m128i *) (ImgVerde->imageData + fila * ImgVerde->widthStep);
        __m128i *pImgAzul = (__m128i *) (ImgAzul->imageData + fila * ImgAzul->widthStep);
        for (cc = 0; cc < ImgOrigen->widthStep; cc = cc + 16) {
            *pImgRojo = _mm_and_si128(rojo, *pImgOrigen);
            *pImgVerde = _mm_and_si128(verde, *pImgOrigen);
            *pImgAzul = _mm_and_si128(azul, *pImgOrigen);
            *pImgRojo++;
            *pImgVerde++;
            *pImgAzul++;
            *pImgOrigen++;
        }
    }

    cvShowImage("Img ROJO", ImgRojo);
    cvWaitKey(0);
    cvReleaseImage(&ImgRojo);
    cvShowImage("Img VERDE", ImgVerde);
    cvWaitKey(0);
    cvReleaseImage(&ImgVerde);
    cvShowImage("Img AZUL", ImgAzul);
    cvWaitKey(0);
    cvReleaseImage(&ImgAzul);

    // Self-explanatory
    cvDestroyWindow(argv[1]);

    return EXIT_SUCCESS;
}
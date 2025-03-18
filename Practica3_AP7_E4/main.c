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
    IplImage* ImgAlpha = cvCreateImage(cvSize(ImgOrigen->width, ImgOrigen->height), IPL_DEPTH_8U, 4);

    cvShowImage("Img ORIGEN", ImgOrigen);
    cvWaitKey(0);

    __m128i alphaColor = _mm_set1_epi32(0xFF000000);
    __m128i alphaRojo = _mm_set1_epi32(0x00800000);

    int fila, cc;
    for (fila = 0; fila < ImgOrigen->height; fila++) {
        __m128i *pImgOrigen = (__m128i *) (ImgOrigen->imageData + fila * ImgOrigen->widthStep);
        __m128i *pImgAlpha = (__m128i *) (ImgAlpha->imageData + fila * ImgAlpha->widthStep);
        for (cc = 0; cc < ImgOrigen->widthStep; cc = cc + 16) {
            __m128i alphaImg = _mm_and_si128(alphaColor, *pImgOrigen);
            __m128i alphaAzul = _mm_srli_epi32(alphaImg, 8);
            __m128i alphaVerde = _mm_srli_epi32(alphaAzul, 8);
            alphaImg = _mm_or_si128(alphaImg, alphaAzul);
            alphaImg = _mm_or_si128(alphaImg, alphaVerde);
            alphaImg = _mm_or_si128(alphaImg, alphaRojo);
            *pImgAlpha = alphaImg;
            *pImgAlpha++;
            *pImgOrigen++;
        }
    }

    cvShowImage("Img ROJO", ImgAlpha);
    cvWaitKey(0);
    cvReleaseImage(&ImgAlpha);

    // Self-explanatory
    cvDestroyWindow(argv[1]);

    return EXIT_SUCCESS;
}
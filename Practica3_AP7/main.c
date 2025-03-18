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

    IplImage* ImgDestino = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 4);

    cvNamedWindow("Img FUNDIDA", CV_WINDOW_AUTOSIZE);

    int fila, cc;
    for (fila = 0; fila < ImgDestino->height; fila++) {
        __m128i *pImgDestino = (__m128i *) (ImgDestino->imageData + fila * ImgDestino->widthStep);
        for (cc = 0; cc < ImgDestino->widthStep; cc = cc + 16) {
            *pImgDestino = _mm_set1_epi32(0xA4C639);
            *pImgDestino++;
        }
    }
    cvShowImage("Img FUNDIDA", ImgDestino);
    cvWaitKey(0);

    IplImage* ImgDestino2 = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 4);

    cvNamedWindow("Img FUNDIDA", CV_WINDOW_AUTOSIZE);

    int fila2, cc2;
    for (fila2 = 0; fila2 < ImgDestino2->height; fila2++) {
        __m128i *pImgDestino2 = (__m128i *) (ImgDestino2->imageData + fila2 * ImgDestino2->widthStep);
        for (cc2 = 0; cc2 < ImgDestino->widthStep; cc2 = cc2 + 16) {
            *pImgDestino2 = _mm_set1_epi32(16113744);
            *pImgDestino2++;
        }
    }
    cvShowImage("Img FUNDIDA", ImgDestino2);
    cvWaitKey(0);

    cvReleaseImage(&ImgDestino);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente BLUE");
    cvDestroyWindow("Componente GREEN");
    cvDestroyWindow("Componente RED");

    return EXIT_SUCCESS;
}
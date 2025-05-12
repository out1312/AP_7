// Version do mosaico CON implentacion de THREADS

#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>
#include <pthread.h> /* POSIX Threads */

#define NTHREADS 2

void mosaico_thread(void *ptr, IplImage* Img1, IplImage* Img2, int alto, int ancho) {
    int *zonaPtr;
    zonaPtr = (int*) ptr;
    int zona = *zonaPtr;
    int tamBloque = 16;
    int fila, columna;
    int filaCMP, columnaCMP;

    for (fila = alto / 2 * zona; fila < (zona + 1) * alto / 2; fila = fila + 16) {
        for (columna = 0; columna < Img1->width; columna += tamBloque) {
            int minDiff = 195841;
            int minBloqueFil, minBloqueCol;
            for (filaCMP = 0; filaCMP < Img2->height; filaCMP += tamBloque) {
                for (columnaCMP = 0; columnaCMP < Img2->width; columnaCMP += tamBloque) {
                    int resComp = compararBloques(filaCMP, columnaCMP, Img1, fila, columna, Img2, Img1->height, Img1->width);
                    //printf("%d\n", resComp);
                    if (resComp < minDiff) {
                        minDiff = resComp;
                        minBloqueFil = filaCMP;
                        minBloqueCol = columnaCMP;
                    }
                }
            }
            copiarBloque(minBloqueFil, minBloqueCol, Img1, fila, columna, Img2, Img2->height, Img2->width);
        }
    }
}

void copiarBloque(int filaO, int colO, IplImage* imgO, int filaD, int colD, IplImage* imgD, int alto, int ancho) {
    int tamBloque = 16;
    int colOCP = colO;
    int colDCP = colD;
    int limFilaO = filaO + tamBloque;
    int limColO = colO + tamBloque;
    for (filaO; filaO < limFilaO; filaO++) {
        unsigned char *pImag1 = (unsigned char *) (imgO->imageData + filaO * imgO->widthStep);
        unsigned char *pImag2 = (unsigned char *) (imgD->imageData + filaD * imgD->widthStep);
        colO = colOCP;
        colD = colDCP;
        pImag1 += colO * 3;
        pImag2 += colD * 3;
        for (colO; colO < limColO; colO++) {
            *pImag2++ = *pImag1++;
            *pImag2++ = *pImag1++;
            *pImag2++ = *pImag1++;
            colD++;
        }
        filaD++;
    }
}

int compararBloques(int fila1, int col1, IplImage* img1, int fila2, int col2, IplImage* img2, int alto, int ancho) {
    int tamBloque = 16;
    int col1CP = col1;
    int col2CP = col2;
    int valorComp = 0;
    int limFila1 = fila1 + tamBloque;
    int limCol1 = col1 + tamBloque;
    for (fila1; fila1 < limFila1; fila1++) {
        unsigned char *pImag1 = (unsigned char *) (img1->imageData + fila1 * img1->widthStep);
        unsigned char *pImag2 = (unsigned char *) (img2->imageData + fila2 * img2->widthStep);
        col1 = col1CP;
        col2 = col2CP;
        pImag1 += col1 * 3;
        pImag2 += col2 * 3;
        for (col1; col1 < limCol1; col1++) {
            int pixel;
            for (pixel = 0; pixel < 3; pixel++) {
                valorComp += abs(*pImag2++ - *pImag1++);
            }
            col2++;
        }
        fila2++;
    }
    return valorComp;
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

    pthread_t threads[NTHREADS];
    int filas[NTHREADS];
    int indice1, indice2;
    
    for (indice1 = 0; indice1 < NTHREADS; indice1++) {
        filas[indice1] = indice1;
        printf("\nEl thread %d sustituye las filas %d", indice1, filas[indice1]);
        pthread_create(&threads[indice1], NULL, (void *) &mosaico_thread, (void *) &filas[indice1]);
    }

    for (indice2 = 0; indice2 < NTHREADS; indice2++) {
        pthread_join(threads[indice2], NULL);
    }
    
    cvShowImage(argv[1], Img1);
    cvShowImage(argv[2], Img2);
    cvWaitKey(0);

    int zona = 0;
    mosaico_thread((void*) &zona, Img1, Img2, Img1->height, Img1->width);
    cvShowImage(argv[2], Img2);
    cvWaitKey(0);
    zona = 1;
    mosaico_thread((void*) &zona, Img1, Img2, Img1->height, Img1->width);
    cvWaitKey(1);

    //int resultado = compararBloques(0, 0, Img1, 0, 0, Img1, 768, 1024);
    //printf("Resultado da comparación de bloques: %d\n", resultado);
    cvShowImage(argv[2], Img2);
    cvWaitKey(0);
    return (EXIT_SUCCESS);
}

// Librerie necessarie
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
 
using namespace cv;
using namespace std;

// Variabili globali 
Mat src, src_gray;
int thresh = 80; //Soglia di differenza nella scala di grigi
int max_thresh = 255; //Massima soglia nella scala di grigi che vogliamo identificare

// Funzione di supporto
void corner_Harris( int, void* );
// Funzione principale
int main(int argc, char** argv )
{
    VideoCapture cap(0);
    if(!cap.isOpened())
     return -1;
    for(;;)
    {
        cap >> src;
        cvtColor( src, src_gray, CV_BGR2GRAY );

        // Creazione di una finestra con presenza di trackbar per regolare threshold
        namedWindow( "Source image", CV_WINDOW_AUTOSIZE );
        createTrackbar( "Threshold: ", "Source image", &thresh, max_thresh, corner_Harris );
        resize(src, src, Size(src.cols/2, src.rows/2));
        imshow ( "Source image", src);
        
        corner_Harris( 0, 0);

        if(waitKey(10) >= 0) break;
    }
    return(0);
}
// Funzione cornerHarris 
void corner_Harris( int, void* )
{
    // Definizione come matrici delle variabili dst, dst_norm, dst_norm_scaled
    Mat dst, dst_norm, dst_norm_scaled, copy;
    copy = src_gray.clone();
    //Inizializzazione di dst a matrice di zeri delle stesse dimensioni di src
    dst = Mat::zeros( src.size(), CV_32FC1 );
    // Parametri da passare alla funzione di Harris-Stephens 
    int blockSize = 15; // Dimensioni del blocco nelle vicinanze del pixel di interesse
    int apertureSize = 1; // Dimensione estesa del kernel Sobel dovrebbe essere 1 3 5 o 7
    double k = 0.04;  // parametro libero del detector di Harris
    
    /* Nota: Alla funzione che segue bisogna passare una matrice single-channel ad 
     * 8-bit oppure una matrice di punti "floating"
     * La funzione seguente usa il metodo di riconoscimento dei spigoli di Harris. In
     * maniera simile alle funzioni cornerMinEigenVal() e cornerEigenValandVecs(), per
     * pixel di coordinate (x,y) la funzione calcola una matrice di covarianza di 
     * dimensioni 2x2 a partire da un blocco di dimensioni blockSizexblockSize e infine
     * compie l'operazione di valutazione del dst che avrà la stessa dimensione di src
    */ 

    // Riconoscimento corners, l'ultimo parametro è il metodo di estrazione dei pixel
    cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
 
    // Normalizzazione
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    //convertScaleAbs( dst_norm, dst_norm_scaled );
 
    // Ciclo necessario per disegnare cerchi attorno i corners
    for( int j = 0; j < dst_norm.rows ; j++ )
    { for( int i = 0; i < dst_norm.cols; i++ )
    {
        if( (int) dst_norm.at<float>(j,i) > thresh )
        {
           circle( copy, Point( i, j ), 4,  Scalar(0), 2, 8, 0 );
        }
    }
    }
    // Mostrare i risultati
    //resize(dst_norm_scaled, dst_norm_scaled, Size(dst_norm_scaled.cols/2, dst_norm_scaled.rows/2));
    namedWindow( "Corners window", CV_WINDOW_AUTOSIZE );
    imshow( "Corners window", copy );
}

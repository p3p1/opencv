// Librerie Necessarie 
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

// Variabili globali
Mat src, src_gray;
int maxCorners = 23; //Numero di corner che vogliamo identificare
int maxTrackbar = 2000; //Massimo numero di corners che vogliamo identificare

RNG rng(12345);

// Funzione di supporto
void corner_ShiTomasi( int, void* );

// Funzione principale
int main( int argc, char** argv )
{
      
      src = imread( argv[1], 1 );
      cvtColor( src, src_gray, CV_BGR2GRAY );

      // Creazione di una finestra con presenza di una trackbar per regolare numero di
      // corners 
      namedWindow( "Source image", CV_WINDOW_AUTOSIZE );
      createTrackbar( "Max corners: ", "Source image", &maxCorners, maxTrackbar, corner_ShiTomasi );
      // resize(src, src, Size(src.cols/2, src.rows/2));
      imshow( "Source image", src );

      corner_ShiTomasi( 0, 0);

      waitKey(0);
      return(0);
}

// Funzione 
void corner_ShiTomasi( int, void* )
{
  // Imposto il numero massimo di corners che voglio che mi vengano riportarti in 
  // output dalla funzione goodFeaturesToTrack. Uguale ad 1 se inferiore ad 1
  if( maxCorners < 1 ) { maxCorners = 1; }

  // Parametri da passare alla funzione di Shi-Tomasi necessari per il riconoscimento
  // dei corners in particolare distanza tra blocchi, dimensioni di questi, parametro
  // k
  vector<Point2f> corners; // Inizializzazione matrice corners
  double qualityLevel = 0.01; // Impostazione del livello di qualità dei corners. 
  // Il migliore livello di qualità tra i cornes viene moltiplicato per questo parametro
  // definendo così una soglia che verrà poi impiegata per poter accettare o 
  // rifiutare un corner in funzione se la sua qualità è superiore o inferiore a tale
  // soglia

  double minDistance = 10; // Distanza euclidiana minima tra due corners 
  int blockSize = 3; // Dimensioni del blocco necessario per ogni pixel dove calcolare
  // la matrice di covarianza 
  bool useHarrisDetector = false; //Imposto che il detector di Harris non venga usato
  double k = 0.04;
  /* Nota: Alla funzione che segue bisogna passare una matrice ad 8-bit o floating-point
   * a 32-bit, single-channel.
   * La funzione che segue usa il metodo Shi-Tomasi calcolando la qualità del corner
   * per ogni pixel dell'immagine sorgente usando le funzioni cornerMinEigenVal() o 
   * cornerHarris().
   */
  /// Copia dell'immagine sorgente
  Mat copy;
  copy = src.clone();

  // Identificazione dei corners
  // I corners vengono riportati all'interno del matrice 'corners' 
  goodFeaturesToTrack( src_gray,
               corners,
               maxCorners,
               qualityLevel,
               minDistance,
               Mat(),
               blockSize,
               useHarrisDetector,
               k );

  // Ciclo necessario per disegnare i corners
  cout<<"** Number of corners detected: "<<corners.size()<<endl;
  int r = 4;
  for( int i = 0; i < corners.size(); i++ )
     { circle( copy, corners[i], r, Scalar(rng.uniform(0,255), rng.uniform(0,255),
              rng.uniform(0,255)), -1, 8, 0 ); }

  // Mostrare i corners
  // resize( src_gray, src_gray, Size(src_gray.cols/2, src_gray.rows/2));
  namedWindow( "Corners window", CV_WINDOW_AUTOSIZE );
  imshow( "Corners window", copy );
}



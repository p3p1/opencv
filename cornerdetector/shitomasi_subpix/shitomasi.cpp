/* 
 * The following code compute the corner detection with Shi - Tomasi method. The
 * output of program is two different windows: one with original image and a trackbar
 * to change the threshold of scale of gray, and an another window with image in 
 * gray scale and with the corners detected (represented by circles).
 */

// Necessary library
#include "opencv2/highgui/highgui.hpp" // Library for build a GUI 
#include "opencv2/imgproc/imgproc.hpp" // Library for image processing 
#include <iostream> // Library to have a channel of comunication (load image, videos
// and so on) 
#include <stdio.h> 
#include <stdlib.h>
 
using namespace cv;
using namespace std;

// Global variables  
Mat src, src_gray; // src: matrix where we store the image, src_gray: matrix in scale
//of gray
int maxCorners = 23; //Max number of corners that we want identify
int maxTrackbar = 2000; //Maximum of trackbar

RNG rng(12345);

// Header function
void corner_ShiTomasi( int, void* ); 

// Main function 
int main( int argc, char** argv )
{
  // Load image and conversion in grayscale 
  src = imread( argv[1], 1 );
  cvtColor( src, src_gray, CV_BGR2GRAY );

  // Generation of window with trackbar to change the maximum number of corners
  // that we want identify 
  namedWindow( "Source image", CV_WINDOW_AUTOSIZE ); // Window name
  createTrackbar( "Max corners: ", "Source image", &maxCorners, maxTrackbar, corner_ShiTomasi );
  imshow( "Source image", src ); // Associate the image to window

  corner_ShiTomasi( 0, 0); // Call of header function

  waitKey(0);
  return(0);
}

// corner_ShiTomasi function
void corner_ShiTomasi( int, void* )
{
  // Setting of maximum number of corners that we want identify equal to 1 (if is
  // smaller than 1)
  if( maxCorners < 1 ) { maxCorners = 1; }

  // Parameters of goodFeaturesToTrack 
  vector<Point2f> corners; // Vector where we store the corners
  double qualityLevel = 0.01; // Quality level of corners  
  /* The best quality level between all corners detected times this paramater define
   * a threshold necessary to accept or reject a corner (it depends by the corner
   * quality if is smaller or greather than this threshold)
   */

  double minDistance = 10; // Minimum euclidian distance between two corners 
  int blockSize = 3; // Block dimension to compute the covariance matrix
  bool useHarrisDetector = false; // If we want use the Harris detector
  double k = 0.04; // Free parameter of Harris detector
  /* Note: the input of goodFeaturesToTrack is a 8-bit or 32-bit matrix or floating-
   * point matrix, single-channel
   */
  // Copy of source image
  Mat copy;
  copy = src.clone();

  // Corners are reported in 'corners' vector 
  goodFeaturesToTrack( src_gray,
               corners,
               maxCorners,
               qualityLevel,
               minDistance,
               Mat(),
               blockSize,
               useHarrisDetector,
               k );

  // For-cycle necessary to draw the detected corners
  int r = 4; // Radius of circles
  for( int i = 0; i < corners.size(); i++ )
     { circle( copy, corners[i], r, Scalar(rng.uniform(0,255), rng.uniform(0,255),
              rng.uniform(0,255)), -1, 8, 0 ); }

  // Creation of window to plot the results of the goodFeaturesToTrack function
  // resize( src_gray, src_gray, Size(src_gray.cols/2, src_gray.rows/2));
  namedWindow( "Corners window", CV_WINDOW_AUTOSIZE );
  imshow( "Corners window", copy );

  // Set the parameters to find the refined corners
  Size winSize = Size ( 5, 5 );
  Size zeroZone = Size ( -1, -1 );
  // Define the class for termination criteria for iterative algorithms, 40 = mumber
  // of iterations or elements to compute, 0.001 desired accurancy (parameters for iterative
  // algorithm stops), TermCriteria::COUNT and TermCriteria::EPS type of termination
  // criteria may be only TermCriteria::EPS or TermCriteria::COUNT 
  TermCriteria criteria = TermCriteria( TermCriteria::EPS + TermCriteria::COUNT, 40, 0.001 );

  // Calculate the refined corner locations
  cornerSubPix( src_gray, corners, winSize, zeroZone, criteria );
  // Write the refined corner
  for( size_t i = 0; i < corners.size(); i++ )
     { cout<<" -- Refined Corner ["<<i<<"] ("<<corners[i].x<<","<<corners[i].y<<")"<<endl; }
}



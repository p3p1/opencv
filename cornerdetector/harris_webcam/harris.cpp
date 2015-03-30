/* 
 * The following code compute the corner detection with Harris - Stephens method. The
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
int thresh = 70; // Threshold in scale of gray that we want detect
int max_thresh = 255; // Maximum threshold (necessary for trackbar) 

// Header function 
void corner_Harris( int, void* );
// Main function 
int main(int argc, char** argv )
{
    VideoCapture cap(0); // Capture video from internal camera (webcam)
    if(!cap.isOpened()) // Check to control if the capture is correctly opened
     return -1;
    for(;;) // For-cycle necessary to operate on each frame
    {
        cap >> src; // Associate current frame to src matrix
        cvtColor( src, src_gray, CV_BGR2GRAY ); // Conversion in grayscale

         // Generation of window with track to change the threshold 
        namedWindow( "Source image", CV_WINDOW_AUTOSIZE ); // Window name
        createTrackbar( "Threshold: ", "Source image", &thresh, max_thresh, corner_Harris );
        resize(src, src, Size(src.cols/2, src.rows/2)); // Necessary to change window size
        imshow ( "Source image", src);  // Associate the image to window
        
        corner_Harris( 0, 0); // Call of header function

        if(waitKey(10) >= 0) break; // Refresh
    }
    return(0);
}

// corner_Harris funtcion
void corner_Harris( int, void* )
{
    // Maxtri dst, dst_norm, dst_norm_scaled
    Mat dst, dst_norm, dst_norm_scaled;
    //Inizialization of zeros matrix dst with the same dimension of src 
    dst = Mat::zeros( src.size(), CV_32FC1 );
    // Parameters of Harris - Stephens method 
    int blockSize = 10; // Block dimension of pixel neighborhood 
    int apertureSize = 7; // Extended dimension of kernel Sobel:1 3 5 o 7
    double k = 0.03;  // Free parameter of Hariss detector    
    /* Note: the input of cornerHarris must be a single-channel matrix of 8-bit or 
     * floating point matrix
     * The following funtcion use the Harris corner detector method, in the way 
     * similar to functions cornerMinEigenVal() and cornerEigenValandVecs(), for each
     * pixel of coordinates (x,y) the function compute the squared coviarance matrix
     * with dimension of 2x2 start from a block of dimension blockSizexblockSize 
    */ 

    // Corners detection the last paramenter is the extraction method of the pixel
    cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
    
    // Normalization and subsequent conversion in absolute scale
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    // For-cycle necessary to draw the circles around the corners
    for( int j = 0; j < dst_norm.rows ; j++ )
    { for( int i = 0; i < dst_norm.cols; i++ )
    {
        if( (int) dst_norm.at<float>(j,i) > thresh )
        { // The radius is 10   
           circle( src_gray, Point( i, j ), 10,  Scalar(0), 2, 8, 0 );
        }
    }
    }
    // Creation of window to plot the results of cornerHarris function
    //resize(dst_norm_scaled, dst_norm_scaled, Size(dst_norm_scaled.cols/2, dst_norm_scaled.rows/2));
    namedWindow( "Corners window", CV_WINDOW_AUTOSIZE );
    imshow( "Corners window", src_gray);
}

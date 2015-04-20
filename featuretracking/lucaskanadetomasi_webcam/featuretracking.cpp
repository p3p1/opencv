/* 
 * The following code compute at first of all the corner detection with Shi - Tomasi
 * method (with accurancy of subpixel), after of this detection it's provide the 
 * Lucas - Kanade method where we identify the corners tracked and finally it's draw the
 * circles around the corners and provides the swap of corners tracked
 */ 
#include "opencv2/video/tracking.hpp" // Library for tracking function (LK in this case)
#include "opencv2/imgproc/imgproc.hpp" // Library for image processing
#include "opencv2/videoio/videoio.hpp" // Library for video processing
#include "opencv2/highgui/highgui.hpp" // Library for build a GUI

#include <iostream> // Library to have a channel of communication (load image, videos
// and so on) 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{

    // Some inizialization 
    // Criteria to corner detectors 
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    // Size of windows for sub-pixel accurancy 
    Size subPixWinSize(10,10), winSize(31,31);
    Mat gray, prevGray, image, frame; // Matrices for images
    int MAX_COUNT = 500; // Max number of corners that we want identify
    int blockSize = 3; // Block dimension to compute the covariance matrix 
    double minDistance = 10; // Minimum euclidian distance between two corners
    bool useHarrisDetector = false; // If we want use the Harris detector
    double qualityLevel = 0.01; // Quality level of corners
    vector<Point2f> points[2]; // Vector where we store the corners 
    double k = 0.04; // Free parameter of Harris detector
    
    int levelpyramid = 3; // Level of pyramid in the LK method: 0 single level, 1
    //two levels, and so on

    VideoCapture cap(0); // Capture video from internal camera (webcam)

    if( !cap.isOpened() ) // Check to control if the capture os correctly opened
    {
        cout << "Could not initialize capturing...\n";
        return 0;
    }

    namedWindow( "LK", 1 ); //Inizialize the window LK where we report the output

    for(;;) // For-cycle necessary to operate of each frame
    {
        cap >> frame; // Associate current frame
        if( frame.empty() ) // Check to control if the frame is empty
            break;

        frame.copyTo(image); // Copy of frame in new matrix
        cvtColor(image, gray, COLOR_BGR2GRAY); // Conversion in grayscale
        // Corners are stored in the second vector before inizialized 
        goodFeaturesToTrack(gray,
                points[1], 
                MAX_COUNT, 
                qualityLevel, 
                minDistance, 
                Mat(), 
                blockSize, 
                useHarrisDetector, 
                k );
        cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
        
        if( !points[0].empty() ) // Check to control if points vector isn't empty
        {
            vector<uchar> status; // Each element of the vector is set to 1 if the flow for the corresponding features has been found, otherwise, it is set to 0.
            vector<float> err; // Outup vector of errors; each element of the vector is set to an error
            //for the corresponding feature; it the flow wasn't found then the error is not defined 
            if(prevGray.empty())
                gray.copyTo(prevGray);
            calcOpticalFlowPyrLK(prevGray, // Previous image in sequence in grayscale
                    gray, // Next image in sequence in grayscale
                    points[0], // Vector of 2D points for which the flow needs to be found 
                    points[1], // Vector of 2D points containing the calculated new positions of
                    //input features in the second image
                    status, 
                    err, 
                    winSize,
                    levelpyramid, 
                    termcrit, 
                    0, // Don't use the initial estimations stored in the points[1]
                    0.001); // The algorithm calculates the minimum eigen value of a 
                    //2x2 normal matrix of optical flow equations (this matrix is 
                    //called a spatial gradient matrix), divided by number of pixels 
                    //in a window; if this value is less than minEigThreshold, then 
                    //a corresponding feature is filtered out and its flow is not 
                    //processed, so it allows to remove bad points and get a 
                    //performance boost.
            
            size_t i, k; // Define two indices for scroll the points vector, size_t
            //stands for the unsigned integer type of the result of the sizeof 
            //operator. It can store the maximum size of a theoretically possible
            //object of any type (including array)
            for( i = k = 0; i < points[1].size(); i++ )
            {
                if( !status[i] ) // Check if status(i) is different from 0
                    continue;
                points[1][k++] = points[1][i]; // Save the corner tracked
                circle( image, points[1][i], 3, CV_RGB(255,0,0), -1, 8); // Draw
                //circles on the image color is red and radius is 3, 8 is linetype,
                //-1, is thickness
            }
            points[1].resize(k);
        }

        imshow("LK", image); //Report image inside the window LK
        if(waitKey(10) >=0 ) break; // Wait 10 millisec before refresh
        std::swap(points[1], points[0]); // Swap between the two vectors of corners
        cv::swap(prevGray, gray); // Swap between the images previous and next
    }

    return 0;
}
/*vim: set ft=cpp ts=4 sw=4 tw=0 et :*/

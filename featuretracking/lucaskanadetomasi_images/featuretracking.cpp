#include <iostream>
#include <ctype.h>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string> // Library necessary to generate a string from variables
#include <sstream> // Library necessary to generate a string from variables 
#include <opencv2/imgproc/imgproc.hpp> // Library for image processing in openCv
#include <opencv2/core/core.hpp> // Library for main functions of openCV (imread,imwrite,...)
#include <opencv2/features2d.hpp> // Library for calcOpticalFlowPyrLK function
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
/**
 * The following function use the main functions of openCV to rotate an image of 
 * input the call of function is the following (let us consider src as source image
 * and dst the output image (rotates)) Mat dst; rotate (src, "angle", dst)
 */
void rotate(cv::Mat& src, double angle, cv::Mat& dst)
{
    int len = std::max(src.cols, src.rows);
    cv::Point2f pt(len/2., len/2.);
    cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);

    cv::warpAffine(src, dst, r, cv::Size(len, len));
}
// Main function 
int main( int argc, char** argv )
{   
    vector<String> filenames; // Inizialization of strings vector where stores the
                              //path of the images in a specific folder
    glob( "images/*.bmp", filenames); // With this function it's possible save the path
                                      //all images inside the folder "images" in the
                                      //vector before inizialized
    // Parameters for goodFeaturesToTrack: for more information see the cornerdetection
    //folder
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3;
    double k = 0.04;
    const int MAX_COUNT = 30;
    // Parameters for draw the arrows in last images
    int thicknessline = 2; // Thickness of line
    int tiplength=0.1; // Lenght of tip
    // Criteria for the goodFeaturesToTrack function
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    // Size of window for sub pixel accurancy
    Size subPixWinSize(10,10), winSize(31,31);

    char buffer[1000];// String output where write the path of output images

    int factorscale = 1; // Factor scale for images output
    int u = 0; // Parameter to take in account only 5 images for time

    while(u < filenames.size()) // Condition that allows to scan the images inside 
        //images directory only 5 images for time
    {
        Mat img3 = imread(filenames[u]); // Store in img3 (the output image) the first
        //image of the five images
        resize( img3, img3, Size(img3.cols/factorscale, img3.rows/factorscale)); // Resize
        rotate(img3, 180, img3); // Rotate

        vector<Point2f> points1[filenames.size()]; // Inizialize multiple vectors where stores the coordinates of features detect from goodFeaturesToTrack function
        vector<Point2f> points2[filenames.size()];

        vector<uchar> status[filenames.size()]; // Status of features recognize or not between the two images
        vector<float> err[filenames.size()]; // Error of calcOpticalFlowPyrLK function
        for(size_t i = u; i < u+4; i++) // For cycle to keep two images for time (of five images selected before)
        {
            Mat img1 = imread(filenames[i]);
            Mat img2 = imread(filenames[i+1]);

            if( !img1.data || !img2.data ) // Check if two images are correct loaded
                cerr << "Problems loading images"<< endl;
        
            resize( img1, img1, Size(img1.cols/factorscale, img1.rows/factorscale)); // Resize
            resize( img2, img2, Size(img2.cols/factorscale, img2.rows/factorscale));
            rotate( img1, 180, img1 ); // Rotate
            rotate( img2, 180, img2 );

            cvtColor( img1, img1, CV_BGR2GRAY ); // Conversion in gray-scale
            cvtColor( img2, img2, CV_BGR2GRAY );

        // Recognize the features
        goodFeaturesToTrack( img1, points1[i], MAX_COUNT, qualityLevel, minDistance, Mat(), blockSize, 0, k);
        //Increase accurancy (sub-pixel)
        cornerSubPix( img1, points1[i], subPixWinSize, Size(-1,-1), termcrit);
        goodFeaturesToTrack( img2, points2[i], MAX_COUNT, qualityLevel, minDistance, Mat(), blockSize, 0, k);
        cornerSubPix( img2, points2[i], subPixWinSize, Size(-1,-1), termcrit);
        
        // Application of Lucas-Kanade method
        calcOpticalFlowPyrLK( img1, img2, points1[i], points2[i], status[i], err[i], winSize, 3, termcrit, 0, 0.001);
        // Draw an arrow between the points tracked
            for( int j=0; j < status[i].size(); j++ )
            {
                if( !status[i][j] )
                    continue;
                Point p0( ceil (points1[i][j].x), ceil (points1[i][j].y));
                Point p1( ceil (points2[i][j].x), ceil (points2[i][j].y));
                arrowedLine( img3, p0, p1, CV_RGB(255,0,0), thicknessline, tiplength);
            }
        
        }
    sprintf(buffer, "output/img%u.jpg", u/5); // Generation of string with variable u as part of name
    imwrite( buffer, img3 ); // Save images based string before generated
    u=u+5; // Select the next five images
    }
    return 0;
}

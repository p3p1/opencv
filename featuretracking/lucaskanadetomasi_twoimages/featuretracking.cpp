#include <iostream>
#include <ctype.h>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void help(char argv)
{
    cout << "\nThis program gets you started reading a sequence of images using cv::VideoCapture.\n"
      << "Image sequences are a common way to distribute video data sets for computer vision.\n"
      << "Usage: " << argv[0] << " <path to the first image in the sequence>\n"
      << "example: " << argv[0] << " right%%02d.jpg\n"
      << "q,Q,esc -- quit\n"
      << "\tThis is a starter sample, to get you up and going in a copy paste fashion\n"
      << endl;
}
int main( int argc, char** argv )
{
    
    if(argc !=2 )
    {
        help(argv)
        return 1;
    }
    string arg = argv[1];
    Videocapture sequence(arg);
    if (!sequence.isOpened())
    {
        cerr << "Failed to open Image Sequence!\n" << endl;
        return 1;
    }
    for(;;)
    {
        sequence;>>image
    
    
    
    }
    Mat image; 
    int factorscale = 2;
    
    if(img1.empty() || img2.empty())
    {
        printf("Can't read one of the images\n");
        return -1;
    }
    resize( img1, img1, Size(img1.cols/factorscale, img1.rows/factorscale));
    resize( img2, img2, Size(img2.cols/factorscale, img2.rows/factorscale));

    Size imgsz = img1.size();
    Mat img3(imgsz,1), img_fin(imgsz,1);
    
    img1.copyTo(img3);
    
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    Size subPixWinSize(10,10), winSize(31,31);

    // Parameters for goodFeaturesToTrack 
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3;
    double k = 0.04;
    const int MAX_COUNT = 500;
    
    vector<Point2f> points1;
    vector<Point2f> points2;

    goodFeaturesToTrack( img1, points1, MAX_COUNT, qualityLevel, minDistance, Mat(), blockSize, 0, k);
    cornerSubPix( img1, points1, subPixWinSize, Size(-1,-1), termcrit);
    goodFeaturesToTrack( img2, points2, MAX_COUNT, qualityLevel, minDistance, Mat(), blockSize, 0, k);
    cornerSubPix( img2, points2, subPixWinSize, Size(-1,-1), termcrit);
    
    vector<uchar> status;
    vector<float> err;
    calcOpticalFlowPyrLK( img1, img2, points1, points2, status, err, winSize, 3, termcrit, 0, 0.001);
    
    int thicknessline = 2;
    int tiplength=0.1;

    cvtColor(img3, img3, CV_GRAY2BGR);  
    
    for( int i=0; i < status.size(); i++ )
    {
        if( !status[i] )
            continue;
        Point p0( ceil (points1[i].x), ceil (points1[i].y));
        Point p1( ceil (points2[i].x), ceil (points2[i].y));
        arrowedLine( img3, p0, p1, CV_RGB(255,0,0), thicknessline, tiplength);
    }
    namedWindow( "Corners window", 1 );
    imshow( "Corners window", img3 );

    waitKey(0);
    return 0;
}

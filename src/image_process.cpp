#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include<string>

#include "include/image_process.h"

using namespace cv;
using namespace std;

CountRice::CountRice()
    : rice_number_(0)
{

}

void CountRice::countRice(Mat image, Mat& result_image)
{
    process(image, result_image);
}

void CountRice::process(Mat image, Mat& result_image)
{
    Mat process_image = image.clone();
    result_image = image.clone();
    Mat gray_image, back_image;
//    GaussianBlur(process_image, process_image, Size(3, 3), 2, 2 );
    cvtColor(process_image, gray_image, CV_BGR2GRAY);

    erode(gray_image, back_image, Mat(), Point(-1, -1), 5);
    gray_image = gray_image - back_image;

    erode(gray_image, gray_image, Mat(), Point(-1, -1), 1);
    dilate(gray_image, gray_image, Mat(), Point(-1, -1), 1);
//    imshow("gray_image", gray_image);
//    imshow("back", back_image);
//    cvWaitKey(-1);

    threshold(gray_image, gray_image, 0, 255.0, CV_THRESH_BINARY | THRESH_OTSU);

    vector<vector<Point>> contours;
    findContours(gray_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area > 10)
        {
            rice_number_++;
            drawContours(result_image, contours, i, Scalar(0, 255, 0));
//            putText(result_image, to_string(rice_number_), contours[i][0], 1, 0.5, Scalar(0, 0, 255));
        }
    }
}

int CountRice::riceNumber()
{
    return rice_number_;
}

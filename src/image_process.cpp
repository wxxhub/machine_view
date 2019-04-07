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
    cvtColor(process_image, gray_image, CV_BGR2GRAY);

    erode(gray_image, back_image, Mat(), Point(-1, -1), 5);
    gray_image = gray_image - back_image;
    imwrite("-gray_image.jpg", gray_image);
    threshold(gray_image, gray_image, 0, 255.0, CV_THRESH_BINARY | THRESH_OTSU);

    erode(gray_image, gray_image, Mat(), Point(-1, -1), 1);
    dilate(gray_image, gray_image, Mat(), Point(-1, -1), 1, 4);
    erode(gray_image, gray_image, Mat(), Point(0, 0), 1);
//    dilate(gray_image, gray_image, Mat(), Point(-1, -1), 1, 1);

    imwrite("gray_image.jpg", gray_image);
    imwrite("back.jpg", back_image);

    vector<vector<Point>> contours;
    findContours(gray_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    double max_area = 0, max_length = 0;
    int max_area_site = 0,max_length_site = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        double length = arcLength(contours[i], 1);
        if (area > 0)
        {
            if (area > max_area)
            {
                max_area = area;
                max_area_site = i;
            }

            if (length > max_length)
            {
                max_length = length;
               max_length_site = i;
            }
            rice_number_++;
            drawContours(result_image, contours, i, Scalar(0, 255, 0));
        }
    }

    putText(result_image, "max_area", contours[max_area_site][0], 1, 1, Scalar(0, 0, 255));
    drawContours(result_image, contours, max_area_site, Scalar(0, 0, 255));

    putText(result_image, "max_length", contours[max_length_site][0], 1 , 1, Scalar(0, 255, 255));
    drawContours(result_image, contours, max_length_site, Scalar(0, 255, 255));

    printf("max_area: %f, position: (%d, %d)\n", max_area, contours[max_area_site][0].x,contours[max_area_site][0].y);
    printf("max_length: %f, position: (%d, %d)\n", max_length, contours[max_length_site][0].x,contours[max_length_site][0].y);
}

int CountRice::riceNumber()
{
    return rice_number_;
}

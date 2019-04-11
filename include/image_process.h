#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#include <opencv2/highgui/highgui.hpp>

class CountRice
{
public:
    CountRice();

    void countRice(cv::Mat image, cv::Mat& result_image);
    void process(cv::Mat image, cv::Mat& result_image);
    int riceNumber();

private:

    int rice_number_;
};

class ImageProcess
{
public:
    ImageProcess();

    std::list<cv::Mat> cutImage(cv::Mat image, int cols_num, int rows_num);
};

class NumDet
{
public:
    NumDet();


private:
//    std::

};

#endif // IMAGE_PROCESS_H

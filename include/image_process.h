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

#endif // IMAGE_PROCESS_H

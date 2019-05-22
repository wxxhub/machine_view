#ifndef WXXPCA_H
#define WXXPCA_H

#include <opencv2/highgui/highgui.hpp>
#include <eigen3/Eigen/Dense>

class WxxPCA
{
public:
    WxxPCA();
    bool readImages(std::string link_link_path);
    void dataProcess();

    void setTempleSize(int x, int y);

    void test();
    void detector(cv::Mat image);

    void imshowTest();
private:
    std::vector<cv::Mat> images_;
    std::vector<cv::Mat> test_images_;

    Eigen::MatrixXd train_matrix_;
    Eigen::MatrixXd test_matrix_;

    Eigen::MatrixXd train_mean_;
    Eigen::MatrixXd train_cov_;
    Eigen::MatrixXd train_result_;

    std::vector<int> train_lab_;
    std::vector<int> test_lab_;

    int temple_x_;
    int temple_y_;

    int dimension_;

    // 向量化
    void vectorizeImage();
    void calculateMatrix();

    int random(int start, int end);
};

#endif // WXXPCA_H

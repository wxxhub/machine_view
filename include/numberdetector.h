#ifndef NUMBERDETECTOR_H
#define NUMBERDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <list>

#define TEMPLE_LEN 7
#define RESIZE_LEN 28
//#define UINT_LEN RESIZE_LEN/TEMPLE_LEN
const int  UINT_LEN = RESIZE_LEN/TEMPLE_LEN;

#define MIN_RATE 0.3
#define alpha 0.05
#define AMP_FACTOR 2

const std::string PJI_FILE_NAME = "pji.txt";

class NumberDetector
{
public:
    NumberDetector();

    // 重载模板
    void reloadTemple();

    // 创建模板
    void createTemple(std::string directory, int num);
    int createBayesTemple(std::string directory);  //创建贝叶斯模板

    // 模板匹配
    float match(cv::Mat image, int &result);
    float byesMatch(cv::Mat image, int &result);

    // 统计结果
    float detectStatistics(std::string directory, int num);
    float byesDetectStatistics(std::string directory, int num);

    void analysisPJI();

private:
    // 模板处理
    void templeProcess(std::list<cv::Mat> mat_list, int num);
    void templeBayesProcess(std::list<cv::Mat> mat_list, int num);

    // 打印结果
    void printTemple(uchar temple[][TEMPLE_LEN], int mat_size);
    void printByesTemple(std::list<std::list<uchar>> temple_list);
    void printPJI();

    // 重置模板
    void resetTemple(uchar temple[][TEMPLE_LEN], int mat_size);
    void resetBayesTemple(uchar temple[][TEMPLE_LEN], int mat_size);

    // 后验概率处理
    double posteriorProbabilite();
    double getMaxPosProb(int &result);

    void savePJI();

    // 用于模板匹配的模板组
    cv::Mat temple_group_[10];

    float prior_probabilaty_[10] = {0};     // 先验概率
    double class_condition_probabilaty_[10] = {0};  // 类条件概率
    double postertior_probability_[10] = {0};   // 后验概率
    double p_j_i_[TEMPLE_LEN*TEMPLE_LEN][10] = {0}; // Pj(wi)
};

#endif // NUMBERDETECTOR_H

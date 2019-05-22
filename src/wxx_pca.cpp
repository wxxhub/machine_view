#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

#include "include/wxx_pca.h"

using namespace std;
using namespace cv;
using namespace Eigen;

static inline std::string &ltrim(std::string &s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}
static inline std::string &rtrim(std::string &s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}
static inline std::string &trim(std::string &s)
{
  return ltrim(rtrim(s));
}

static inline std::vector<std::string> split(const std::string &text, char sep)
{
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;

  while ((end = text.find(sep, start)) != (std::string::npos))
  {
    tokens.push_back(text.substr(start, end - start));
    trim(tokens.back());
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  trim(tokens.back());

  return tokens;
}

WxxPCA::WxxPCA()
{
    temple_x_ = 0;
    temple_y_ = 0;
    dimension_= 40;
}

bool WxxPCA::readImages(string link_path)
{
    ifstream link_file(link_path);

    if(!link_file.is_open())
    {
       return false;
    }

    images_.clear();
    test_images_.clear();
    test_lab_.clear();
    train_lab_.clear();

    string read_line;
    int test_i = 0;
    while(getline(link_file, read_line))
    {
        std::vector<std::string> v = split(read_line, ' ');
        string image_path = v[0];
        Mat image = imread(image_path, CV_LOAD_IMAGE_ANYDEPTH);
        if (!image.empty())
        {
#ifdef IMAGE_DEBUG
            imshow("image", image);
            cvWaitKey(10);
#endif /* IMAGE_DEBUG */
            if (temple_x_ == 0 || temple_y_ == 0)
            {
                temple_x_ = image.cols;
                temple_y_ = image.rows;
            }
            if (test_i == 5)
            {
                test_images_.push_back(image);
                test_lab_.push_back(atoi(v[1].c_str()));
                test_i = 0;
            }
            else
            {
                images_.push_back(image);
                train_lab_.push_back(atoi(v[1].c_str()));
            }

             test_i++;
        }
        else
        {
            printf("open image failed!\n");
        }
   }

    if (images_.size() <= 1)
        return false;

    return true;
}

void WxxPCA::dataProcess()
{
    vectorizeImage();
    calculateMatrix();
}

void WxxPCA::vectorizeImage()
{
    train_matrix_ = MatrixXd::Zero(images_.size(), temple_x_*temple_y_);
    test_matrix_ = MatrixXd::Zero(test_images_.size(), temple_x_*temple_y_);
    int size = temple_x_*temple_y_;
    /*
     *   @@@@@@@@@@@ image0
     *   ########### image1
     *   $$$$$$$$$$$ image2
     *   ...
     */

    vector<Mat>::iterator iter = images_.begin();
    for (int i = 0; iter != images_.end(); iter++, i++)
    {
        Mat reshape_image = iter->reshape(0, 1);
        u_char* data = reshape_image.data;
        for (int j = 0; j < size; j++)
            train_matrix_.coeffRef(i, j) = data[j];
    }

    iter = test_images_.begin();
    for (int i = 0; iter != test_images_.end(); iter++, i++)
    {
        Mat reshape_image = iter->reshape(0, 1);
        u_char* data = reshape_image.data;
        for (int j = 0; j < size; j++)
            test_matrix_.coeffRef(i, j) = data[j];
    }
}

void WxxPCA::calculateMatrix()
{
    train_mean_ = train_matrix_.colwise().mean();
    RowVectorXd meanVecRow(RowVectorXd::Map(train_mean_.data(),train_matrix_.cols()));
    MatrixXd zero_mean_mat = train_matrix_;
    zero_mean_mat.rowwise() -= meanVecRow;

    //计算协方差
    MatrixXd C = zero_mean_mat * zero_mean_mat.transpose();

    Eigen::SelfAdjointEigenSolver<MatrixXd> eigenSolver(C);

    // 取值最大的dimension_个数据
//    MatrixXd D = eigenSolver.eigenvalues().bottomRows(dimension_);
    MatrixXd V = eigenSolver.eigenvectors().rightCols(dimension_);

//    cout<<"D: "<<D.cols()<<" "<<D.rows()<<endl;
//    cout<<"V: "<<V.cols()<<" "<<V.rows()<<endl;

    train_cov_ = zero_mean_mat.transpose() * V;
    /*
     * 1 3 5
     * 2 4 6
     */

    MatrixXd train_cov_square = train_cov_.array().square();

    for (int i = 0; i < train_cov_.cols(); i++)
    {
        train_cov_.col(i) = train_cov_.col(i)/sqrt(train_cov_square.col(i).sum());
    }
    train_result_ = zero_mean_mat * train_cov_;
}

void WxxPCA::test()
{
    RowVectorXd meanVecRow(RowVectorXd::Map(train_mean_.data(),test_matrix_.cols()));

    MatrixXd temple_mat = test_matrix_;
    temple_mat.rowwise() -= meanVecRow;

    MatrixXd date_test_new = temple_mat * train_cov_;

    vector<Mat>::iterator iter = test_images_.begin();
    int sum_test = 0, success_test = 0, success_test2 = 0;
    for (int i = 0; iter != test_images_.end(); iter++, i++)
    {
        double min_distance = -1;
        double min_sceond_distance = -1;
        int mat_sit = 0;
        int mat_sit2 = 0;
        MatrixXd test_mat = date_test_new.row(i);
        int size = images_.size();
        for (int j = 0; j < size; j++)
        {
            MatrixXd train_result_row = train_result_.row(j);
            MatrixXd diff_nat = train_result_row - test_mat;
            double dis = diff_nat.row(0).squaredNorm();
            if (min_distance < 0 || min_distance > dis)
            {
                min_sceond_distance = min_sceond_distance;
                min_distance = dis;
                mat_sit2 = mat_sit;
                mat_sit = j;
            }
        }

//        cout<<"min_distance: "<<min_distance<<endl;
        sum_test++;
        if (test_lab_[i] == train_lab_[mat_sit])
        {
            success_test++;
            char test_result_path[24];
            char train_result_path[24];
            sprintf(test_result_path, "test_result/%d_test.jpg", i);
            sprintf(train_result_path, "test_result/%d_train.jpg", i);
            imwrite(test_result_path, test_images_[i]);
            imwrite(train_result_path, images_[mat_sit]);
        }
        else
        {
            char test_result_path[32];
            char train_result_path[32];
            sprintf(test_result_path, "test_failed/%d_test.jpg", i);
            sprintf(train_result_path, "test_failed/%d_train.jpg", i);
            imwrite(test_result_path, test_images_[i]);
            imwrite(train_result_path, images_[mat_sit]);
        }
    }
    cout<<"sum_test: "<<sum_test<<" success_test: "<<success_test<<" success_test2: "<<success_test2<<endl;
}

void WxxPCA::imshowTest()
{
    vector<Mat>::iterator iter = test_images_.begin();
    for (; iter != test_images_.end(); iter++)
    {
        imshow("test image", *iter);
        cvWaitKey(-1);
    }
}

int WxxPCA::random(int start, int end)
{
    return start+rand()%(end-start+1);
}

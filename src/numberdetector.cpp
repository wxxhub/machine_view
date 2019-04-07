#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "include/numberdetector.h"

using namespace std;
using namespace cv;

NumberDetector::NumberDetector()
{
    reloadTemple();
}

void NumberDetector::reloadTemple()
{
    for (int i = 0; i < 10; i++)
    {
        std::string temple_name = to_string(i)+"_temple.bmp";
        temple_group_[i] = imread(temple_name, CV_LOAD_IMAGE_GRAYSCALE);
        if (temple_group_[i].empty())
        {
            printf("load temple: %d failed", i);
            continue;
        }
        threshold(temple_group_[i], temple_group_[i], 125, 255, CV_THRESH_BINARY);
    }
}

void NumberDetector::createTemple(std::string directory, int num)
{
    string temple_head = directory+ "/" + to_string(num)+"_";
    int back_num = 0;
    list<Mat> mat_list;
    while (true)
    {
        string temple_path = temple_head + to_string(back_num) + ".bmp";

        Mat image = imread(temple_path, CV_LOAD_IMAGE_GRAYSCALE);
        if (image.empty())
        {
            templeProcess(mat_list, num);
            mat_list.clear();
            break;
        }

        threshold(image, image, 125, 255, CV_THRESH_BINARY);
        mat_list.push_back(image);
        cvWaitKey(10);

        back_num++;
    }
}

int NumberDetector::createBayesTemple(string directory)
{
    memset(prior_probabilaty_, 0, sizeof(prior_probabilaty_));
    memset(p_j_i_, 0, sizeof(p_j_i_));
    int image_sum = 0;
    for (int i = 0; i < 10; i++)
    {
        string temple_head = directory+ "/" + to_string(i)+"_";
        int back_num = 0;
        list<Mat> mat_list;
        while (true)
        {
            string temple_path = temple_head + to_string(back_num) + ".bmp";

            Mat image = imread(temple_path, CV_LOAD_IMAGE_GRAYSCALE);
            if (image.empty())
            {
                templeBayesProcess(mat_list, i);
                mat_list.clear();
                break;
            }

            threshold(image, image, 125, 255, CV_THRESH_BINARY);
            mat_list.push_back(image);

            prior_probabilaty_[i] += 1.0;
            image_sum++;
            back_num++;
        }
    }

    for (int i = 0; i < 10; i++)
    {
        prior_probabilaty_[i] = prior_probabilaty_[i]/image_sum;
        printf("%d prior_probabilaty: %f\n",i, prior_probabilaty_[i]);
    }
}

void NumberDetector::templeProcess(std::list<Mat> mat_list, int num)
{
    if (mat_list.size() <= 0)
        return;

    uchar temple[TEMPLE_LEN][TEMPLE_LEN] = {0};
    uchar save_temple[RESIZE_LEN][RESIZE_LEN] = {0};

    std::string temple_name = to_string(num) + "_temple.bmp";
    list<Mat>::iterator iter = mat_list.begin();
    for (; iter != mat_list.end(); iter++)
    {
        uchar *p_data = iter->data;
        for (int x = 0; x < RESIZE_LEN; x++)
        {
            for (int y = 0; y < RESIZE_LEN; y++)
            {
                int value = p_data[x * RESIZE_LEN+y];

                if (value >= 125)
                {
                    temple[x/4][y/4] += 1;
                }
            }
        }
    }
    printf("temple: %d\n", num);
    printTemple(temple, mat_list.size());
    resetTemple(temple, mat_list.size());

    for (int x = 0; x < RESIZE_LEN; x++)
    {
        for (int y = 0; y < RESIZE_LEN; y++)
        {
            save_temple[x][y] = int(temple[x/UINT_LEN][y/UINT_LEN]);
        }
    }
    Mat temple_image = Mat(RESIZE_LEN, RESIZE_LEN, CV_8U, save_temple);
    imwrite(temple_name, temple_image);

#ifdef IMAGE_DEBUG
    imshow("test2", temple_image);
    cvWaitKey(10);
#endif /* IMAGE_DEBUG */
}

void NumberDetector::templeBayesProcess(std::list<Mat> mat_list, int num)
{
    if (mat_list.size() <= 0)
        return;

    memset(class_condition_probabilaty_, 0, sizeof(class_condition_probabilaty_));
    list<list<uchar>> temple_list;
    uchar save_temple[RESIZE_LEN][RESIZE_LEN] = {0};

    std::string temple_name = to_string(num) + "_temple.bmp";
    list<Mat>::iterator iter = mat_list.begin();

    int Ni = mat_list.size();
    for (; iter != mat_list.end(); iter++)
    {
        uchar temple[TEMPLE_LEN][TEMPLE_LEN] = {0};
        uchar *p_data = iter->data;
        for (int x = 0; x < RESIZE_LEN; x++)
        {
            for (int y = 0; y < RESIZE_LEN; y++)
            {
                int value = p_data[x * RESIZE_LEN+y];

                if (value >= 125)
                {
                    temple[x/4][y/4] += 1;
                }
            }
        }

        list<uchar> templei;
        for (int i = 0; i < TEMPLE_LEN*TEMPLE_LEN; i++)
        {
            int x = i / TEMPLE_LEN;
            int y = i % TEMPLE_LEN;
            if (temple[x][y]>1)
                templei.push_back(1);
            else
                templei.push_back(0);
        }
        temple_list.push_back(templei);
    }

    printf("w(%d):\n", num);
    printByesTemple(temple_list);
    // Pj(wi)
    list<list<uchar>>::iterator temple_iter = temple_list.begin();

    // for k=1:pattern(i).num
    for (; temple_iter != temple_list.end();temple_iter++)
    {
        list<uchar>::iterator temple_iter0 = temple_iter->begin();
        for (int j = 0; temple_iter0 != temple_iter->end(); temple_iter0++, j++)
        {
            p_j_i_[j][num] += *temple_iter0;
        }
    }

    // calculate p_j_i
    for (int j = 0; j < TEMPLE_LEN*TEMPLE_LEN; j++)
    {
        p_j_i_[j][num] = (p_j_i_[j][num]+1.0)/(Ni+2.0);
        printf("p_j_i_[%d][%d]: %-3.5f", j, num, p_j_i_[j][num]);
    }
    printPJI();
}

float NumberDetector::match(Mat image, int &result)
{
    double min_distance = -1;
    result = 0;
    for (int i = 0; i < 10; i++)
    {
        if(temple_group_[i].empty())
        {
            printf("no temple\n");
            continue;
        }

        double distance = norm(image, temple_group_[i], CV_L2);
        if (min_distance == -1)
            min_distance = distance;
        else if(min_distance > distance)
        {
            min_distance = distance;
            result = i;
        }
//        printf("%d distance %f", i, distance);
    }

}

float NumberDetector::byesMatch(Mat image, int &result)
{
    uchar temple[TEMPLE_LEN][TEMPLE_LEN] = {0};
    uchar *p_data = image.data;
    for (int x = 0; x < RESIZE_LEN; x++)
    {
        for (int y = 0; y < RESIZE_LEN; y++)
        {
            if(p_data[x*RESIZE_LEN+y] > 125)
                temple[x/4][y/4] += 1;
        }
    }

    // print temple

    printf("temple:\n");
    for (int i = 0; i < TEMPLE_LEN; i++)
    {
        for (int j = 0; j < TEMPLE_LEN; j++)
        {
            printf("%5d", temple[i][j]);
        }
        printf("\n");
    }

    double max_pro = 0;
    for (int i = 0; i < 10; i++)
    {
        double sum = 1;
        for (int j = 0; j < TEMPLE_LEN*TEMPLE_LEN; j++)
        {
            int x = j/TEMPLE_LEN;
            int y = j%TEMPLE_LEN;
            if (temple[x][y] > 1)
                sum *= p_j_i_[j][i]*AMP_FACTOR;
            else
                sum *= (1-p_j_i_[j][i])*AMP_FACTOR;
        }

        printf("\n");
        if (max_pro < sum)
        {
            max_pro = sum;
//            result = i;
        }
        class_condition_probabilaty_[i] = sum;
        printf("[%d]: %20.15f\n", i, sum);
    }
    posteriorProbabilite();
    return getMaxPosProb(result);
}

float NumberDetector::detectStatistics(string directory, int num)
{
    string temple_head = directory+ "/" + to_string(num)+"_";
    int success_times = 0;

    // 图片尾数
    int back_num = 0;
    list<Mat> mat_list;
    while (true)
    {
        string temple_path = temple_head + to_string(back_num) + ".bmp";

        Mat image = imread(temple_path, CV_LOAD_IMAGE_GRAYSCALE);
        if (image.empty())
        {
            if (back_num == 0)
                return -1;

            return 1.0 * success_times / back_num;
        }

        int result = -1;
        match(image, result);

        if (result == num)
            success_times++;

        back_num++;
    }
}

float NumberDetector::byesDetectStatistics(string directory, int num)
{
    string temple_head = directory+ "/" + to_string(num)+"_";
    int success_times = 0;

    // 图片尾数
    int back_num = 0;
    list<Mat> mat_list;
    while (true)
    {
        string temple_path = temple_head + to_string(back_num) + ".bmp";

        Mat image = imread(temple_path, CV_LOAD_IMAGE_GRAYSCALE);
        if (image.empty())
        {
            if (back_num == 0)
                return -1;

            return 1.0 * success_times / back_num;
        }

        int result = -1;
        byesMatch(image, result);

        if (result == num)
            success_times++;

        back_num++;
    }
}

void NumberDetector::printTemple(uchar temple[][TEMPLE_LEN], int mat_size)
{
    int sum = mat_size * UINT_LEN * UINT_LEN;
    printf("number: \n");
    for (int x = 0; x < TEMPLE_LEN; x++)
    {
        for (int y = 0; y < TEMPLE_LEN; y++)
        {
            printf("%8d", temple[x][y]);
        }
        printf("\n");
    }

    printf("rate: \n");
    for (int x = 0; x < TEMPLE_LEN; x++)
    {
        for (int y = 0; y < TEMPLE_LEN; y++)
        {
            printf("%8.3f", 1.0*temple[x][y]/sum);
//            printf("%8d", sum);
        }
        printf("\n");
    }
    printf("\n");
}

void NumberDetector::printByesTemple(list<list<uchar> > temple_list)
{
    list<list<uchar>>::iterator iter = temple_list.begin();
    for (; iter != temple_list.end(); iter++)
    {
        list<uchar>::iterator iter2 = iter->begin();
        for (; iter2 != iter->end(); iter2++)
        {
            printf("%6d", *iter2);
        }
        printf("\#\n");
    }
}

void NumberDetector::printPJI()
{
    printf("\n");
    for (int i = 0; i < 10; i++)
    {
        printf("\n\nPJI: %d\n", i);
        for (int j = 0; j < TEMPLE_LEN*TEMPLE_LEN; j++)
        {
            if(j%TEMPLE_LEN == 0)
                printf("\n");
            printf("%25.15f", p_j_i_[j][i]);
        }
    }
    printf("\n");
}

void NumberDetector::resetTemple(uchar temple[][TEMPLE_LEN], int mat_size)
{
    int sum = mat_size * UINT_LEN * UINT_LEN;
    //reset
    for (int x = 0; x < TEMPLE_LEN; x++)
    {
        for (int y = 0; y < TEMPLE_LEN; y++)
        {
            if (1.0*temple[x][y]/sum < MIN_RATE)
            {
                temple[x][y] = 0;
            }
            else
            {
                temple[x][y] = 255;
            }
            printf("%8d", temple[x][y]);
        }
        printf("\n");
    }
}

void NumberDetector::resetBayesTemple(uchar temple[][TEMPLE_LEN], int mat_size)
{
    int sum = mat_size * UINT_LEN * UINT_LEN;

    for (int x = 0; x < TEMPLE_LEN; x++)
    {
        for (int y = 0; y < TEMPLE_LEN; y++)
        {
            if (1.0*temple[x][y]/sum < 0.05)
            {
                temple[x][y] = 0;
            }
            else
            {
                temple[x][y] = 255;
            }
            printf("%8d", temple[x][y]);
        }
        printf("\n");
    }
}

double NumberDetector::posteriorProbabilite()
{
    double sum = 0;

    for (int i = 0; i < 10; i++)
    {
        sum += class_condition_probabilaty_[i]*prior_probabilaty_[i];
    }

    for (int i = 0; i < 10; i++)
    {
        postertior_probability_[i] = class_condition_probabilaty_[i]*prior_probabilaty_[i]/sum;
        printf("postertior_probability_[%d]: %-6.15f\n",i, postertior_probability_[i]);
    }
}

double NumberDetector::getMaxPosProb(int &result)
{
    double max = 0;
    for (int i = 0; i < 10; i++)
    {
        if (max < postertior_probability_[i])
        {
            max = postertior_probability_[i];
            result = i;
        }
    }
}

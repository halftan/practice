#include "ExImage.h"

ExImage ExImage::sharp()
{
    int height, width;
    float v1, v2;
    height = img_mat.size().height;
    width = img_mat.size().width;

    if (img_mat.type() != CV_32F)
        img_mat.convertTo(img_mat, CV_32F);

    normalize(img_mat, img_mat, 0, 1, CV_MINMAX);
    cv::Mat tmp = img_mat.clone();
    for (int y = 1; y < height-1; y++){
        for (int x = 1; x < width-1; x++){
            //水平方向的sobel处理
            v1 = abs( (img_mat.at<float>(y-1,x+1) + 2*img_mat.at<float>(y,x+1) + img_mat.at<float>(y+1,x+1)) - (img_mat.at<float>(y-1,x-1) + 2*img_mat.at<float>(y,x-1) + img_mat.at<float>(y+1,x-1)) );
            //垂直方向的sobel处理
            v2 = abs( (img_mat.at<float>(y+1,x-1) + 2*img_mat.at<float>(y+1,x) + img_mat.at<float>(y+1,x+1)) - (img_mat.at<float>(y-1,x-1) + 2*img_mat.at<float>(y-1,x) + img_mat.at<float>(y-1,x+1)) );
            // 求水平和垂直方向上sobel处理后的值
            tmp.at<float>(y,x) = v1 + v2;
        }
    }
    normalize(tmp, img_mat, 0, 1, CV_MINMAX);
    return *this;
}

ExImage ExImage::smooth()
{
    int height, width;
    float v;
    height = img_mat.size().height;
    width = img_mat.size().width;

    if (img_mat.type() != CV_32F)
        img_mat.convertTo(img_mat, CV_32F);

    normalize(img_mat, img_mat, 0, 1, CV_MINMAX);
    for (int y = 1; y < height-1; y++){
        for (int x = 1; x < width-1; x++){
            v = img_mat.at<float>(y-1,x-1) + img_mat.at<float>(y-1,x) + img_mat.at<float>(y-1,x+1) + 
                img_mat.at<float>(y,x-1) + img_mat.at<float>(y,x) + img_mat.at<float>(y,x+1) +
                img_mat.at<float>(y+1,x-1) + img_mat.at<float>(y+1,x) + img_mat.at<float>(y+1,x+1);
            v = v / 9.f;
            img_mat.at<float>(y,x) = v;
        }
    }
    normalize(img_mat, img_mat, 0, 1, CV_MINMAX);
    return *this;
}

ExImage ExImage::lowPass()
{
    int h,w,r;
    h = fourierI.size().height - 1;
    w = fourierI.size().width - 1;
    r = w / 5;
    cv::Mat tmp = cv::Mat::zeros(fourierI.size(), CV_32FC2);

    for (int v = 0; v < r; ++v) {
        int u = sqrt((float)(r*r - v*v)) + 0.5;
        for (; u >= 0; --u) {
            tmp.at<cv::Vec2f>(v, u)[0] = fourierI.at<cv::Vec2f>(v, u)[0];
            tmp.at<cv::Vec2f>(v, u)[1] = fourierI.at<cv::Vec2f>(v, u)[1];
            tmp.at<cv::Vec2f>(v, w-u)[0] = fourierI.at<cv::Vec2f>(v, w-u)[0];
            tmp.at<cv::Vec2f>(v, w-u)[1] = fourierI.at<cv::Vec2f>(v, w-u)[1];
            tmp.at<cv::Vec2f>(h-v, u)[0] = fourierI.at<cv::Vec2f>(h-v, u)[0];
            tmp.at<cv::Vec2f>(h-v, u)[1] = fourierI.at<cv::Vec2f>(h-v, u)[1];
            tmp.at<cv::Vec2f>(h-v, w-u)[0] = fourierI.at<cv::Vec2f>(h-v, w-u)[0];
            tmp.at<cv::Vec2f>(h-v, w-u)[1] = fourierI.at<cv::Vec2f>(h-v, w-u)[1];
        }
    }

    fourierI = tmp;
    return *this;
}

ExImage ExImage::highPass()
{
    int h,w,r;
    h = fourierI.size().height - 1;
    w = fourierI.size().width - 1;
    r = w / 5;

    for (int v = 0; v < r; ++v) {
        int u = sqrt((float)(r*r - v*v)) + 0.5;
        for (; u >= 0; --u) {
            fourierI.at<cv::Vec2f>(v, u)[0] = 0;
            fourierI.at<cv::Vec2f>(v, u)[1] = 0;
            fourierI.at<cv::Vec2f>(v, w-u)[0] = 0;
            fourierI.at<cv::Vec2f>(v, w-u)[1] = 0;
            fourierI.at<cv::Vec2f>(h-v, u)[0] = 0;
            fourierI.at<cv::Vec2f>(h-v, u)[1] = 0;
            fourierI.at<cv::Vec2f>(h-v, w-u)[0] = 0;
            fourierI.at<cv::Vec2f>(h-v, w-u)[1] = 0;
        }
    }

    return *this;
}
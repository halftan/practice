#ifndef EX_IMAGE_H
#define EX_IMAGE_H 1

#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define EIMGREAD 1

typedef unsigned int uint;

using namespace std;

class ExImage
{
public:
    string getFullPath() const;
    string getBasename() const;
    void showImage();
    void closeImage();
    ExImage getHistogram();
    ExImage equalizeHist();
    ExImage histogramMatching(ExImage matching);

    cv::Mat getMat() const;
    ExImage(const string path);
    ExImage(const ExImage& src);
    ExImage(const cv::Mat& src);
    ExImage(const cv::Mat& src, const string short_name);
    ExImage operator=(const ExImage& src);

    class EImageError
    {
    public:
        string getMessage();
        unsigned int getErrcode();
        EImageError(string msg, unsigned int code);
    private:
        string msg;
        unsigned int errcode;
    };
    ~ExImage(){};

private:
    string full_path;
    string win_name;
    cv::Mat img_mat;
    string gen_random();
};

typedef ExImage::EImageError ImgErr;

#endif

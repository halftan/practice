#ifndef EX_IMAGE_H
#define EX_IMAGE_H 1

#include <iostream>
#include <complex>
#include <string>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define EIMGREAD 1

typedef unsigned int uint;

using namespace std;

typedef ExImage::EImageError ImgErr;
typedef cv::Mat_<complex<double>> cmat;
typedef complex<double> cmat_elem;

void clearImage(cv::Mat img, int h, int w, uchar t=255);

class ExImage
{
public:
    string getFullPath() const;
    string getBasename() const;
    void showImage();
    void closeImage();

    /*
     * Histogram
     */
    ExImage getHistogram();
    ExImage equalizeHist();
    ExImage histogramMatching(ExImage matching);

    /*
     * Fourier Transformation
     */
    void calcFourier();
    ExImage getFourier();
    ExImage getInverseFourier();

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
    cmat fourier_mat;
    string gen_random();
};

#endif

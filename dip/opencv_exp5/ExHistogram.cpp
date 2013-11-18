#include "ExImage.h"

int calcHist(long *hist, cv::Mat &img)
{
    int max_value = 0;
    for (int y = 0; y < img.rows; ++y) {
        uchar *p = img.ptr(y);
        for (int x = 0; x < img.cols; ++x) {

            hist[p[x]]++;
            if (hist[p[x]] > max_value)
                max_value = hist[p[x]];
        }
    }
    return max_value;
}

void calcCDF(double *cdf, long *hist, long size)
{
    double v = 0.0;

    for(int i = 0; i < 256; i++){
        v += hist[i];
        cdf[i] = v / size;
    }
}

/*
    [out] lut, [in] cdf
*/
void equalizeLUT(uchar *lut, double *cdf)
{
    for(int i = 0; i < 256; i++){
        if (floor(cdf[i]*255+0.5) < 0.0) {
            lut[i] = 0;
        } else if (floor(cdf[i]*255+0.5) > 255.0){
            lut[i] = 255;
        } else lut[i] = (uchar)floor(cdf[i]*255+0.5);
    }
}

/*
    [out] lut, [in] cdf_orig, [in] img_matching
*/
void SML_LUT(uchar *lut, double *cdf_orig, cv::Mat &match)
{
    double min = 0, v = 0, cdf_match[256] = {0.};
    long hist_match[256] = {0}, size_match = 0;

    size_match = match.size().area();

    calcHist(hist_match, match);
    calcCDF(cdf_match, hist_match, size_match);

    for(int j = 0; j < 256; j++){
        min = DBL_MAX;
        for(int i = 0; i < 256; i++){
            v = fabs(cdf_orig[j] - cdf_match[i]);
            if (min > v) {
                min = v;
                lut[j] = i;
            }
        }
    }
}

/*
    [out] dest, [in] src, [in] lut, [in] w, [in] h
*/
void applyLUT(cv::Mat &dest, cv::Mat &src, uchar *lut, int w, int h)
{
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            dest.at<uchar>(y, x) = lut[src.at<uchar>(y, x)];
        }
    }
}


/*
    returns the histogram of the receiver
*/
ExImage ExImage::getHistogram()
{
    long hist[256] = {0}, _hist[256] = {0};
    int max_value = 0, count = 0;
    cv::Mat hist_img;

    hist_img = cv::Mat::zeros(256, 256, CV_8UC1);
    // clearImage(hist_img, 256, 256);
    max_value = calcHist(hist, img_mat);

    for (int i = 0; i < 256; ++i) {
        _hist[i] = 255 * hist[i] / max_value;
    }

    for (int x = 0; x  < 256; ++x)
        for (int y = 255; y > 255 - _hist[x]; --y) {
            hist_img.at<uchar>(y, x) = 127;
        }

    // ExImage ret(hist_img, win_name + " Histogram");
    // return ret;
    return ExImage(hist_img, win_name + " Histogram");
}

ExImage ExImage::equalizeHist()
{
    cv::Mat result;
    int w, h;
    long size, hist[256] = {0};
    uchar lut[256] = {0};
    double cdf[256] = {0};

    w = img_mat.size().width;
    h = img_mat.size().height;
    size = img_mat.size().area();
    result = cv::Mat::zeros(h, w, CV_8UC1);
    // clearImage(result, h, w);

    calcHist(hist, img_mat);

    calcCDF(cdf, hist, size);

    equalizeLUT(lut, cdf);

    applyLUT(result, img_mat, lut, w, h);

    ExImage ret(result, win_name + " HistoEqualized");
    return ret;
}

/*
    直方图规定化
*/
ExImage ExImage::histogramMatching(ExImage matching)
{
    int w, h;
    long hist[256] = {0}, size = 0;
    double cdf[256] = {0.};
    uchar lut[256] = {0};
    cv::Mat result;

    w = img_mat.size().width;
    h = img_mat.size().height;
    size = img_mat.size().area();
    result = cv::Mat::zeros(h, w, CV_8UC1);
    // clearImage(result, h, w);

    calcHist(hist, img_mat);
    calcCDF(cdf, hist, size);
    cv::Mat temp = matching.getMat();
    SML_LUT(lut, cdf, temp);
    applyLUT(result, img_mat, lut, w, h);

    ExImage ret(result, win_name + " HistoMatched");
    return ret;
}

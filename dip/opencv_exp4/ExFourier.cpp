#include "ExImage.h"

void ExImage::calcFourier()
{
    int h, w, k;
    double area;

    h = img_mat.size().height;
    w = img_mat.size().width;
    area = img_mat.size().area();
    k = sqrt(area);

    this->fourier_mat = new cdouble[(long)area];

    for (int v = 0; v < h; ++v) {
        for (int u = 0; u < w; ++u) {
            double t_real = 0, t_imag = 0;
            cout << "U: " << u << "\tV: " << v << endl;
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    double angle = -(double(u*x)/w + double(v*y)/h) * 2 * PI;
                    int s = (x+y) % 2;
                    if (s == 1)
                        s = -1;
                    else
                        s = 1;
                    t_real += s * (img_mat.at<uchar>(y, x) * cos(angle));
                    t_imag += s * (img_mat.at<uchar>(y, x) * sin(angle));
                }
            }
            t_real /= k;
            t_imag /= k;

            this->fourier_mat[v*w+u] = cdouble(t_real, t_imag);
        }
    }
}

ExImage ExImage::convertToFourierImg()
{
    const double C = 0.5;
    int x, y, h, w;
    double temp, min, max, *_img, s, k;

    if (this->fourier_mat == NULL)
        this->calcFourier();

    h = img_mat.size().height;
    w = img_mat.size().width;

    this->img_mat = cv::Mat::zeros(h, w, CV_8UC1);

    _img = new double[w*h];
    max = min = C * log(std::norm(this->fourier_mat[0]) + 1);
    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x) {
            temp = std::norm(this->fourier_mat[y*w + x]);
            _img[y*w + x] = C * log(1 + temp);
            if (_img[y*w + x] > max) {
                max = _img[y*w + x];
            }
            if (_img[y*w + x] < min) {
                min = _img[y*w + x];
            }
        }
    }

    k = 255 / max;
    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x) {
            s = ( _img[y*w + x] - min ) * k + C;
            if (s < 0) {
                s = 0;
            } else if (s > 255) {
                s = 255;
            }
            this->img_mat.at<uchar>(y, x) = (uchar)s;
        }
    }
    delete[] _img;

    return *this;
}

ExImage ExImage::convertToOriginalImg()
{
    if (this->fourier_mat == NULL)
        return *this;

    int w, h;
    double k, area;

    h = img_mat.size().height;
    w = img_mat.size().width;
    area = img_mat.size().area();
    k = sqrt(area);

    this->img_mat = cv::Mat::zeros(h, w, CV_8UC1);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            double rn = 0, vn = 0;

            for (int v = 0; v < h; ++v) {
                for (int u = 0; u < w; ++u) {
                    double angle = (double(u*x)/w + double(v*y)/h) * 2 * PI;

                    rn += this->fourier_mat[v*w + u].real() * cos(angle) -
                        this->fourier_mat[v*w + u].imag() * sin(angle);
                    vn += this->fourier_mat[v*w + u].real() * sin(angle) -
                        this->fourier_mat[v*w + u].imag() * cos(angle);
                }
            }

            rn /= k;
            vn /= k;

            int s = (x + y) % 2;
            if (s == 1) {
                s = -1;
            } else {
                s = 1;
            }
            rn = 2 * rn;

            int result = int(rn + 0.5);
            if (result < 0) result = 0;
            if (result > 255) result = 255;
            this->img_mat.at<uchar>(y,x) = result;
        }
    }

    return *this;
}

#include "ExImage.h"

void ExImage::calcFourier()
{
    int h, w, k;
    double area;

    h = img_mat.size().height;
    w = img_mat.size().width;
    area = img_mat.size().area();
    k = sqrt(area);

    this->real_num = new double[h*w];
	this->imag_num = new double[h*w];

    for (int v = 0; v < h; ++v) {
        cout << "V: " << v << endl;
        for (int u = 0; u < w; ++u) {
            double t_real = 0, t_imag = 0;
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

			this->real_num[v*w + u] = t_real;
			this->imag_num[v*w + u] = t_imag;
        }
    }
}

ExImage ExImage::convertToFourierImg()
{
    const double C = 0.5;
    int x, y, h, w;
    double temp, min, max, *_img, s, k;

    if (this->imag_num == NULL || this->real_num == NULL)
        this->calcFourier();

    h = img_mat.size().height;
    w = img_mat.size().width;

    this->img_mat = cv::Mat::zeros(h, w, CV_8UC1);

    _img = new double[w*h];
    max = min = C * log(std::norm(complex<double>(this->real_num[0], this->imag_num[0])) + 1);
    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x) {
            temp = std::norm(complex<double>(this->real_num[y*w + x], this->imag_num[y*w + x]));
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
    if (this->imag_num == NULL || this->real_num == NULL)
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

                    rn += this->real_num[v*w + u] * cos(angle) -
                        this->imag_num[v*w + u]* sin(angle);
                    vn += this->real_num[v*w + u] * sin(angle) -
                        this->imag_num[v*w + u] * cos(angle);
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
            rn = s * rn;

            int result = int(rn + 0.5);
            if (result < 0) result = 0;
            if (result > 255) result = 255;
            this->img_mat.at<uchar>(y,x) = result;
        }
        cout << "V: " << y << endl;
    }

    return *this;
}

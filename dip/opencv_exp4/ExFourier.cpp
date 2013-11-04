#include "ExImage.h"

void ExImage::calcFourier()
{
    int h, w;
    double area, k;

    h = img_mat.size().height;
    w = img_mat.size().width;
    area = img_mat.size().area();
    k = sqrt(area);

    this->fourier_mat = cmat::zeros(h, w, cmat_elem);

    for (int v = 0; v < h; ++v) {
        for (int u = 0; u < w; ++u) {
            cmat_elem t(0);
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    double angel = -(double(u*x)/w + double(v*y)/h) * 2 * PI;
                    int s = (x+y) % 2;
                    if (s == 1)
                        s = -1;
                    else
                        s = 1;
                    t.real += s * (img_mat.at<uchar>(y, x) * cos(angle));
                    t.imag += s * (img_mat.at<uchar>(y, x) * sin(angle));
                }
            }
            t.read /= k;
            t.imag /= k;

            fourier_mat.at<cmat_elem>(v, u) = t;
        }
    }
}

ExImage ExImage::getFourier()
{

}

ExImage ExImage::getInverseFourier()
{

}

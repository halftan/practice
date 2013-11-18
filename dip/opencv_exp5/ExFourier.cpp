#include "ExImage.h"

using namespace cv;
void ExImage::calcFourier()
{
    Mat channels[] = {Mat_<float>(img_mat), Mat::zeros(img_mat.size(), CV_32F)};
    Mat fourierI;
    merge(channels, 2, fourierI);         // Add to the expanded another plane with zeros

    dft(fourierI, fourierI);            // this way the result may fit in the source matrix
}

ExImage ExImage::convertToFourierImg()
{
    Mat fourierI;
    Mat channels[] = {Mat_<float>(img_mat), Mat::zeros(img_mat.size(), CV_32F)};
    merge(channels, 2, fourierI);

    dft(fourierI, fourierI);

    split(fourierI, channels);
    magnitude(channels[0], channels[1], channels[0]);
    Mat magI = channels[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, img_mat, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).
    return *this;
}

ExImage ExImage::convertToOriginalImg()
{

    return *this;
}

#include "ExImage.h"

/*
    constructor. path is the path of image file.
*/
ExImage::ExImage(const string path)
{
    this->full_path   = path;
    this->win_name    = getBasename();
    this->img_mat     = cv::imread(path, 0);
    this->fourier_mat = NULL;
    //cv::cvtColor(this->img_mat, this->img_mat, CV_BGR2GRAY);
    if (img_mat.empty())
        throw EImageError(
        "Cannot open image or file not exist.", EIMGREAD
        );
}

/*
 * destructor.
 */
ExImage::~ExImage()
{
    delete this->fourier_mat;
}

ExImage::ExImage(const cv::Mat& src)
    : full_path(gen_random()), win_name(gen_random()), img_mat(src) { this->fourier_mat = NULL; }

ExImage::ExImage(const cv::Mat& src, string short_name)
    : full_path(gen_random()), win_name(short_name), img_mat(src) { this->fourier_mat = NULL; }

/*
    copy/assignment constructor
*/
ExImage::ExImage(const ExImage& src)
    : img_mat(src.getMat())
{
    this->full_path   = src.getFullPath();
    this->win_name    = src.getBasename();
    this->fourier_mat = NULL;
}

ExImage ExImage::operator=(const ExImage& src)
{
    this->full_path   = src.getFullPath();
    this->win_name    = src.getBasename();
    this->img_mat     = src.getMat();
    this->fourier_mat = NULL;
    return *this;
}

/*
    getter function of full_path and win_name attribute.
    win_name is the result of getBasename
*/
string ExImage::getBasename() const
{
    int pos;

    // return win_name immediately
    // if win_name is generated before.
    if (this->win_name.length())
        return this->win_name;

    pos = this->full_path.find_last_of('/');
    if (pos == string::npos)
        pos = this->full_path.find_last_of('\\');
    if (pos == string::npos)
        return this->full_path;
    else
        return this->full_path.substr(pos, this->full_path.length() - pos);
}

string ExImage::getFullPath() const
{
    return this->full_path;
}

/*
    delegates to cv::highgui
*/
void ExImage::showImage()
{
    cv::namedWindow(this->win_name);
    cv::imshow(this->win_name, this->img_mat);
}

void ExImage::closeImage()
{
    cv::destroyWindow(this->win_name);
}

/*
    returns the cv::Mat of the image
*/
cv::Mat ExImage::getMat() const
{
    return this->img_mat;
}

/*
    generates random string.
*/
string ExImage::gen_random() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    int len = rand() % 10;
    string str("          ");
    for (int i = 0; i < len; ++i) {
        str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return str;
}

/*
    exception class for ExImage.
*/
ExImage::EImageError::EImageError(string msg, unsigned int code)
{
    this->msg = msg;
    this->errcode = code;
}

string ExImage::EImageError::getMessage()
{
    return this->msg;
}

unsigned int ExImage::EImageError::getErrcode()
{
    return this->errcode;
}

/*
 * helper function to reset image
 */
void clearImage(cv::Mat img, int h, int w, uchar t)
{
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            img.at<uchar>(y, x) = t;
}

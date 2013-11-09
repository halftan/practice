#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "main.h"
#include "ExImage.h"

using namespace std;

int main(int argc, char **argv)
{
    string path;
    ExImage *image = NULL;
    string msg = "Please input the image name";

    path = getImagePath(msg, "../lena.jpg",true);
    while (path.length() != 0 && !(
        path.length() == 1 && path[0] == 'q')) {
        try {
            image = new ExImage(path);

            cout << "Original image:" << endl;

            image->showImage();
            cv::waitKey(0);
            image->closeImage();

            cout << "Fourier Transformed Image:" << endl;
            image->calcFourier();
            image->convertToFourierImg();

            image->showImage();
            cv::waitKey(0);
            image->closeImage();

            cout << "Inverse Fourier Transformed Image:" << endl;
            image->convertToOriginalImg();

            image->showImage();
            cv::waitKey(0);
            image->closeImage();

            delete image;
            image = NULL;
        } catch (ImgErr e) {
            cerr << e.getMessage() << '\n';
        }

        path = getImagePath(msg, "../lena.jpg", true);
    }
    if (image)
        delete image;

    return 0;
}

string getImagePath(const string &msg, const string &def, bool flag)
{
    string name;
    cout << msg << "(Default to \"" << def << "\"):" << endl;
    if (flag)
        cout << "Type 'q' to exit:" << endl;
    getline(cin, name);
    if (name.length() == 0)
        return def;
    return name;
}

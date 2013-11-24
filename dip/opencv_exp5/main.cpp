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
    string msg = "Please input the image name";
    
    for (path = getImagePath(msg, "../lena.jpg",true);
		path.length() != 0 && !(path.length() == 1 && path[0] == 'q');
		path = getImagePath(msg, "../lena.jpg",true)) {
        try {
            ExImage image = ExImage(path);
			ExImage orig = image;
			ExImage four = orig;

            cout << "Original image:" << endl;

            image.showImage();
            cv::waitKey(0);
            image.closeImage();

            cout << "Sharpened Image:" << endl;
			image.sharp();

            image.showImage();
            cv::waitKey(0);
            image.closeImage();

            cout << "Smoothed Image:" << endl;
			image = orig;
            image.smooth();

            image.showImage();
            cv::waitKey(0);
            image.closeImage();

			cout << "Low Pass:" << endl;
			orig.calcFourier();
			image = orig;
			four.calcFourier();
			four.lowPass();
			four.convertToFourierImg();

			image.calcFourier();
			image.lowPass();
			image.convertToOriginalImg();

			four.showImage();
			image.showImage();

			cv::waitKey(0);
			image.closeImage();
			four.closeImage();

			cout << "High pass:" << endl;
			image = orig;
			four = orig;

			image.calcFourier();
			image.highPass();

			four.calcFourier();
			four.highPass();
			four.convertToFourierImg();

			image.showImage();
			four.showImage();

			cv::waitKey(0);
			four.closeImage();
			image.closeImage();

        } catch (ImgErr e) {
            cerr << e.getMessage() << '\n';
        }
    }

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

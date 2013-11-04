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
    string msg = "Please input the image name(Default to \"../lena.jpg\"):";
    string msg_match = "Please input the matching image name(Default to \"../matching.jpg\"):";

    path = getImagePath(msg, "../lena.jpg",true);
    while (path.length() != 0 && !(
        path.length() == 1 && path[0] == 'q')) {
        try {
            image = new ExImage(path);
            ExImage histo = image->getHistogram();

            cout << "Original image and histogram" << endl;
            image->showImage();
            histo.showImage();

            cv::waitKey(0);

            image->closeImage();
            histo.closeImage();

            ExImage eqalized = image->equalizeHist();
            histo = eqalized.getHistogram();

            cout << "HistoEqualized image and histogram" << endl;
            eqalized.showImage();
            histo.showImage();

            cv::waitKey(0);

            eqalized.closeImage();
            histo.closeImage();

            ExImage match(getImagePath(msg_match, "../matching.jpg"));
            ExImage matched = image->histogramMatching(match);
            ExImage match_histo = match.getHistogram();
            histo = matched.getHistogram();

            cout << "HistoMatched image and histogram" << endl;

            match_histo.showImage();
            matched.showImage();
            histo.showImage();

            cv::waitKey(0);

            match_histo.closeImage();
            matched.closeImage();
            histo.closeImage();

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
    cout << msg << endl;
    if (flag)
        cout << "Type 'q' to exit:" << endl;
    getline(cin, name);
    if (name.length() == 0)
        return def;
    return name;
}

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

struct IMX {
    int width, height;
    vector<uint8_t> data;

    // load imx file from disk
    IMX(const string &filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error while opening file: " << filename << endl;
            exit(1);
        }

        // get image width and height
        file.read(reinterpret_cast<char *>(&width), sizeof(width));
        file.read(reinterpret_cast<char *>(&height), sizeof(height));

        // resize image vector to hold pixel data (3 bytes per pixel for rgb)
        data.resize(width * height * 3);

        // get pixel data
        file.read(reinterpret_cast<char *>(data.data()), data.size());

        if (!file) {
            cerr << "Error while reading file" << endl;
            exit(1);
        }
    }

    // convert imx (binary) data to opencv mat
    Mat toMat() {
        return Mat(height, width, CV_8UC3, data.data());
    }
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <image.imx>" << endl;
        return 1;
    }

    string filename = argv[1];
    IMX img(filename);

    Mat image = img.toMat();
    imshow("IMX Viewer", image);
    waitKey(0);

    return 0;
}
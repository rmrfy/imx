#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

struct IMX {
    int width, height;
    vector<uint8_t> data;

    IMX(const string &filename) {
        ifstream file(filename, ios::binary);

        file.read(reinterpret_cast<char *>(&width), sizeof(width));
        file.read(reinterpret_cast<char *>(&height), sizeof(height));

        data.resize(width * height * 3);
        file.read(reinterpret_cast<char *>(data.data()), data.size());
    }

    Mat toMat() {
        return Mat(height, width, CV_8UC3, data.data());
    }
};

int main(int argc, char *argv[]) {
    string filename = argv[1];
    IMX img(filename);

    Mat image = img.toMat();
    imshow("IMX Viewer", image);
    waitKey(0);

    return 0;
}
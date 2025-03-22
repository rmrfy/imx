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
        if (!file) {
            cerr << "Error: Could not open file " << filename << endl;
            exit(1);
        }

        file.read(reinterpret_cast<char *>(&width), sizeof(width));
        file.read(reinterpret_cast<char *>(&height), sizeof(height));

        data.resize(width * height * 3);
        file.read(reinterpret_cast<char *>(data.data()), data.size());

        if (!file) {
            cerr << "Error: Could not read file data" << endl;
            exit(1);
        }
    }

    IMX(int w, int h) : width(w), height(h) {
        data.resize(w * h * 3);
    }

    void save(const string &filename) {
        ofstream file(filename, ios::binary);
        file.write(reinterpret_cast<char *>(&width), sizeof(width));
        file.write(reinterpret_cast<char *>(&height), sizeof(height));
        file.write(reinterpret_cast<char *>(data.data()), data.size());
    }

    Mat toMat() {
        return Mat(height, width, CV_8UC3, data.data()).clone();
    }

    static IMX fromMat(const Mat &image) {
        IMX img(image.cols, image.rows);
        memcpy(img.data.data(), image.data, img.data.size());
        return img;
    }
};

// compress IMX image
void compressIMX(const string &inputFile, const string &outputFile, int quality) {
    IMX imxImage(inputFile);
    Mat image = imxImage.toMat();

    // convert RGB to BGR
    cvtColor(image, image, COLOR_RGB2BGR);

    vector<uchar> buffer;
    vector<int> params = {IMWRITE_JPEG_QUALITY, quality};
    imencode(".jpg", image, buffer, params);

    Mat compressedImage = imdecode(buffer, IMREAD_COLOR);
    if (compressedImage.empty()) {
        cerr << "Error: Compression failed!" << endl;
        exit(1);
    }

    // convert BGR back to RGB before saving as IMX
    cvtColor(compressedImage, compressedImage, COLOR_BGR2RGB);
    IMX compressedIMX = IMX::fromMat(compressedImage);

    compressedIMX.save(outputFile);
    cout << "Compressed " << inputFile << " and saved as " << outputFile << " with quality " << quality << "%" << endl;
}

int main(int argc, char *argv[]) {
    if (argc != 5 || string(argv[1]) != "compress") {
        cout << "Usage: " << argv[0] << " compress --quality <value> <input_file> <output_file>\n";
        return 1;
    }

    string qualityArg = argv[2];
    if (qualityArg.find("--quality") != 0) {
        cerr << "Error: Missing --quality flag\n";
        return 1;
    }

    int quality = stoi(qualityArg.substr(9));
    if (quality < 1 || quality > 100) {
        cerr << "Error: Quality must be between 1 and 100\n";
        return 1;
    }

    string inputFile = argv[3];
    string outputFile = argv[4];

    compressIMX(inputFile, outputFile, quality);
    return 0;
}
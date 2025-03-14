#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

struct IMX {
    int width, height;
    vector<uint8_t> data;

    IMX(int w, int h) : width(w), height(h), data(w * h * 3, 0) {}

    IMX(const string &filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error: Could not open file " << filename << endl;
            exit(1);
        }

        // get image width and height
        file.read(reinterpret_cast<char *>(&width), sizeof(width));
        file.read(reinterpret_cast<char *>(&height), sizeof(height));

        // resize image vector to hold pixel data
        data.resize(width * height * 3);
        file.read(reinterpret_cast<char *>(data.data()), data.size());

        if (!file) {
            cerr << "Error: Could not read file data" << endl;
            exit(1);
        }
    }

    void save(const string &filename) {
        ofstream file(filename, ios::binary);
        file.write(reinterpret_cast<char *>(&width), sizeof(width));
        file.write(reinterpret_cast<char *>(&height), sizeof(height));
        file.write(reinterpret_cast<char *>(data.data()), data.size());
    }

    // convert imx data to opencv mat
    Mat toMat() {
        Mat image(height, width, CV_8UC3, data.data());
        return image.clone();
    }

    // creates imx object from opencv mat
    static IMX fromMat(const Mat &image) {
        IMX img(image.cols, image.rows);
        memcpy(img.data.data(), image.data, img.data.size());
        return img;
    }
};

// function to get file extension from filename
string getFileExtension(const string &filename) {
    size_t pos = filename.find_last_of(".");
    if (pos == string::npos) return "";
    return filename.substr(pos);
}

// function to convert png or jpg to imx
void convertToIMX(const string &inputFile, const string &outputFile) {
    Mat image = imread(inputFile, IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Error: Could not open image file " << inputFile << endl;
        exit(1);
    }

    cvtColor(image, image, COLOR_BGR2RGB);
    IMX imxImage = IMX::fromMat(image);
    imxImage.save(outputFile);
    cout << "Converted " << inputFile << " to " << outputFile << endl;
}

// function to convert imx to png or jpg
void convertFromIMX(const string &inputFile, const string &outputFile) {
    IMX imxImage(inputFile);
    Mat image = imxImage.toMat();
    cvtColor(image, image, COLOR_RGB2BGR);

    vector<int> params;
    string ext = getFileExtension(outputFile);
    if (ext == ".jpg" || ext == ".jpeg") {
        params.push_back(IMWRITE_JPEG_QUALITY);
        params.push_back(95);
    }

    if (!imwrite(outputFile, image, params)) {
        cerr << "Error: Could not save image to " << outputFile << endl;
        exit(1);
    }
    cout << "Converted " << inputFile << " to " << outputFile << endl;
}

int main(int argc, char *argv[]) {
    // validation for cmd line arguements
    if (argc != 4 || string(argv[1]) != "convert") {
        cout << "Usage: " << argv[0] << " convert <input_file> <output_file>\n";
        return 1;
    }

    string inputFile = argv[2];
    string outputFile = argv[3];
    string inputExt = getFileExtension(inputFile);
    string outputExt = getFileExtension(outputFile);

    if ((inputExt == ".png" || inputExt == ".jpg" || inputExt == ".jpeg") && outputExt == ".imx") {
        convertToIMX(inputFile, outputFile);
    } else if (inputExt == ".imx" && (outputExt == ".png" || outputExt == ".jpg" || outputExt == ".jpeg")) {
        convertFromIMX(inputFile, outputFile);
    } else {
        cerr << "Error: Unsupported conversion from " << inputExt << " to " << outputExt << endl;
        return 1;
    }

    return 0;
}
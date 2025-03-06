#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// features / thoughts
// png, jpg to imx converter (one to one pixel mapping)
// imx to jpg,png converter
// command can have flag to no compress / compress (every alternating pixel etc, example, compression quality like 70%, 80%, default being 100%quality meaning no compression)

struct IMX {
    int width, height;
    vector<uint8_t> data;

    IMX(int w, int h) : width(w), height(h), data(w * h * 3, 0) {}

    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        int index = (y * width + x) * 3;
        data[index] = r;
        data[index + 1] = g;
        data[index + 2] = b;
    }

    // save image as imx (binary)
    void save(const string &filename) {
        ofstream file(filename, ios::binary);
        file.write(reinterpret_cast<char *>(&width), sizeof(width));
        file.write(reinterpret_cast<char *>(&height), sizeof(height));
        file.write(reinterpret_cast<char *>(data.data()), data.size());
    }

    // save image as ppm
    void saveAsPPM(const string &filename) {
        ofstream file(filename);
        file << "P3\n"
             << width << " " << height << "\n255\n";

        for (int i = 0; i < data.size(); i += 3) {
            file << (int)data[i] << " " << (int)data[i + 1] << " " << (int)data[i + 2] << " ";
        }
    }
};

int main() {
    // test image
    int width = 200, height = 200;
    IMX img(width, height);

    int blockSize = 50;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            bool isGreen = ((x / blockSize) + (y / blockSize)) % 2 == 0;
            img.setPixel(x, y, 0, isGreen ? 255 : 0, isGreen ? 0 : 255);
        }
    }

    img.save("output.imx");
    img.saveAsPPM("output.ppm");

    cout << "Saved as output.imx and output.ppm\n";

    return 0;
}
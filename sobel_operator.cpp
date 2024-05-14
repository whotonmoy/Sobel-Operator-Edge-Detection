#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

vector<unsigned char> readRawImage(const char* filename, int width, int height);
void saveImage(const char* filename, const vector<unsigned char>& image, int width, int height);
void applySobelOperator(const vector<unsigned char>& inputImage, vector<unsigned char>& outputImage, int width, int height);
void createBinaryEdgeMap(const vector<unsigned char>& gradientImage, vector<unsigned char>& edgeMap, int width, int height, int threshold);

int main() 
{
    // Define image parameters
    // Change these parameters according to the image being tested
    const int width = 280;
    const int height = 280;

    // Threshold for the binary edge map
    const int threshold = 128;

    // Load the input image
    vector<unsigned char> inputImage = readRawImage("building.raw", width, height);

    // Initialize output images
    vector<unsigned char> outputGx(width * height, 0);
    vector<unsigned char> outputGy(width * height, 0);
    vector<unsigned char> outputGradient(width * height, 0);
    vector<unsigned char> outputEdgeMap(width * height, 0);

    // Apply Sobel operators to compute Gx and Gy
    applySobelOperator(inputImage, outputGx, width, height);
    applySobelOperator(inputImage, outputGy, width, height);

    // Create the gradient image [sqrt(Gx^2 + Gy^2)]
    for (int i = 0; i < width * height; ++i) {
        outputGradient[i] = static_cast<unsigned char>(sqrt(
            static_cast<double>(outputGx[i] * outputGx[i] + outputGy[i] * outputGy[i])
        ));
    }

    // Create the binary edge map
    createBinaryEdgeMap(outputGradient, outputEdgeMap, width, height, threshold);

    // Save the output images
    saveImage("Gx_output.raw", outputGx, width, height);
    saveImage("Gy_output.raw", outputGy, width, height);
    saveImage("Gradient_output.raw", outputGradient, width, height);
    saveImage("EdgeMap_output.raw", outputEdgeMap, width, height);

    return 0;
}

// Function to read a raw image file
vector<unsigned char> readRawImage(const char* filename, int width, int height) {
    vector<unsigned char> image(width * height);
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(image.data()), width * height);
        file.close();
    }
    return image;
}

// Function to save the grayscale image
void saveImage(const char* filename, const vector<unsigned char>& image, int width, int height) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(image.data()), width * height);
        file.close();
    }
}

// Apply the Sobel operator to compute gradient
void applySobelOperator(const vector<unsigned char>& inputImage, vector<unsigned char>& outputImage, int width, int height) {
    vector<int> Gx = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
    vector<int> Gy = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int sumGx = 0;
            int sumGy = 0;

            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    int pixel = inputImage[(y + j) * width + (x + i)];
                    int index = (j + 1) * 3 + (i + 1);
                    sumGx += pixel * Gx[index];
                    sumGy += pixel * Gy[index];
                }
            }

            double gradient = sqrt(static_cast<double>(sumGx * sumGx + sumGy * sumGy));
            outputImage[y * width + x] = static_cast<unsigned char>(gradient);
        }
    }
}

// Create a binary edge map with a given threshold
void createBinaryEdgeMap(const vector<unsigned char>& gradientImage, vector<unsigned char>& edgeMap, int width, int height, int threshold) {
    for (int i = 0; i < width * height; ++i) {
        if (gradientImage[i] >= threshold) {
            edgeMap[i] = 255;
        }
        else {
            edgeMap[i] = 0;
        }
    }
}
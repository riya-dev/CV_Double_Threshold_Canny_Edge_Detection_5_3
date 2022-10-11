// cd project5
// cd part3
// g++ -std=c++11 -o l053  -Wall l053.cpp
// ./l053

#include<stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

//#include <cmath>

int width;
int height;
bool** visited;

int** maskx(int** grayscale) {
    int** maskX;
    maskX = new int* [height];
    for (int i = 0; i < height; i++)
        maskX[i] = new int[width];

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            maskX[i][j] = 0;

    for (int i = 1; i < height - 1; ++i) {
        for (int j = 1; j < width - 1; ++j) {
            int a = grayscale[i - 1][j - 1] * -1;
            int b = grayscale[i - 1][j] * 0;
            int c = grayscale[i - 1][j + 1] * 1;

            int d = grayscale[i][j - 1] * -2;
            int e = grayscale[i][j] * 0;
            int f = grayscale[i][j + 1] * 2;

            int g = grayscale[i + 1][j - 1] * -1;
            int h = grayscale[i + 1][j] * 0;
            int k = grayscale[i + 1][j + 1] * 1;

            maskX[i][j] = a + b + c + d + e + f + g + h + k;
        }
    }
    return maskX;
}

int** masky(int** grayscale) {
    int** maskY;
    maskY = new int* [height];
    for (int i = 0; i < height; i++)
        maskY[i] = new int[width];

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            maskY[i][j] = 0;

    for (int i = 1; i < height - 1; ++i) {
        for (int j = 1; j < width - 1; ++j) {
            int a = grayscale[i - 1][j - 1] * -1;
            int b = grayscale[i][j - 1] * 0;
            int c = grayscale[i + 1][j - 1] * 1;

            int d = grayscale[i - 1][j] * -2;
            int e = grayscale[i][j] * 0;
            int f = grayscale[i + 1][j] * 2;

            int g = grayscale[i - 1][j + 1] * -1;
            int h = grayscale[i][j + 1] * 0;
            int k = grayscale[i + 1][j + 1] * 1;

            maskY[i][j] = a + b + c + d + e + f + g + h + k;
        }
    }
    return maskY;
}

void part1() {
    std::ifstream og("image.ppm");

    std::string next;
    if (og.is_open()) {
        og >> next;
        og >> next;
        width = std::stoi(next);
        og >> next;
        height = std::stoi(next);
        og >> next;
        int maxcolor = std::stoi(next);

        int** ogarray;
        ogarray = new int* [height];
        for (int i = 0; i < height; i++)
            ogarray[i] = new int[width];

        std::ofstream imageg;
        imageg.open("imageg.ppm");
        imageg << "P3 " << width << " " << height << " " << maxcolor << "\n";
        // make array
        int** arrayg;
        arrayg = new int* [height];
        for (int i = 0; i < height; i++)
            arrayg[i] = new int[width];

        std::ofstream imagem;
        imagem.open("imagem.ppm");
        imagem << "P3 " << width << " " << height << " 255\n";
        // make array
        int** arraym;
        arraym = new int* [height];
        for (int i = 0; i < height; i++)
            arraym[i] = new int[width];

        int r;
        int g;
        int b;
        int intensity = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                og >> next;
                r = std::stoi(next);
                og >> next;
                g = std::stoi(next);
                og >> next;
                b = std::stoi(next);
                intensity = (int)((r + g + b) / 3);
                arrayg[i][j] = intensity; //intensity
            }
        }
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j)
                imageg << arrayg[i][j] << " " << arrayg[i][j] << " " << arrayg[i][j] << " ";
            imageg << std::endl;
        }
        imageg << "\n";
        imageg.close();

        int** maskX;
        maskX = new int* [height];
        for (int i = 0; i < height; i++)
            maskX[i] = new int[width];

        int** maskY;
        maskY = new int* [height];
        for (int i = 0; i < height; i++)
            maskY[i] = new int[width];

        maskX = maskx(arrayg);
        maskY = masky(arrayg);

        int** magnitude;
        magnitude = new int* [height];
        for (int i = 0; i < height; i++)
            magnitude[i] = new int[width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                magnitude[i][j] = (int)(sqrt((maskX[i][j] * maskX[i][j]) + (maskY[i][j] * maskY[i][j])));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (magnitude[i][j] > 67) //>45
                    imagem << 0 << " " << 0 << " " << 0 << " ";
                else
                    imagem << 255 << " " << 255 << " " << 255 << " ";
            }
            imagem << std::endl;
        }
        imagem << "\n";
        imagem.close();

        og.close();
    }
    else
        std::cout << "Unable to open file";
}

int** recursive(int** threshold, int i, int j) {
    /*
     i - 1, j - 1       i - 1, j        i - 1, j + 1
     i, j - 1           i, j            i, j + 1
     i + 1, j - 1       i + 1, j        i + 1, j + 1
    */
    if (threshold[i][j] == 0)
        return threshold;
    else if (visited[i][j] == true)
        return threshold;
    else {
        visited[i][j] = true;
        if (threshold[i][j] == 1 || threshold[i][j] == 2)
            threshold[i][j] = 2;
        //upper left
        if (visited[i - 1][j - 1] == false)
            recursive(threshold, i - 1, j - 1);
        //top
        if (visited[i - 1][j] == false)
            recursive(threshold, i - 1, j);
        //upper right
        if (visited[i - 1][j + 1] == false)
            recursive(threshold, i - 1, j + 1);
        //left
        if (visited[i][j - 1] == false)
            recursive(threshold, i, j - 1);
        //right
        if (visited[i][j + 1] == false)
            recursive(threshold, i, j + 1);
        //lower left
        if (visited[i + 1][j - 1] == false)
            recursive(threshold, i + 1, j - 1);
        //bottom
        if (visited[i + 1][j] == false)
            recursive(threshold, i + 1, j);
        //lower right
        if (visited[i + 1][j + 1] == false)
            recursive(threshold, i + 1, j + 1);
    }
    return threshold;
}

int** hysteresis(int** threshold) {
    visited = new bool* [height];
    for (int i = 0; i < height; i++)
        visited[i] = new bool[width];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            visited[i][j] = false;

    /*for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            std::cout << threshold[i][j] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";*/

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (threshold[i][j] == 2) {
                visited[i][j] = true;
                threshold = recursive(threshold, i, j);
            }
        }
    }
    return threshold;
}

void part2() {
    std::ifstream og("image.ppm");

    std::string next;
    if (og.is_open()) {
        og >> next;
        og >> next;
        width = std::stoi(next);
        og >> next;
        height = std::stoi(next);
        og >> next;
        int maxcolor = std::stoi(next);

        int** ogarray;
        ogarray = new int* [height];
        for (int i = 0; i < height; i++)
            ogarray[i] = new int[width];

        std::ofstream imageg;
        imageg.open("imageg.ppm");
        imageg << "P3 " << width << " " << height << " " << maxcolor << "\n";
        // make array
        int** arrayg;
        arrayg = new int* [height];
        for (int i = 0; i < height; i++)
            arrayg[i] = new int[width];

        std::ofstream imagem;
        imagem.open("imagem.ppm");
        imagem << "P3 " << width << " " << height << " 255\n";
        // make array
        int** arraym;
        arraym = new int* [height];
        for (int i = 0; i < height; i++)
            arraym[i] = new int[width];

        int r;
        int g;
        int b;
        int intensity = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                og >> next;
                r = std::stoi(next);
                og >> next;
                g = std::stoi(next);
                og >> next;
                b = std::stoi(next);
                intensity = (int)((r + g + b) / 3);
                arrayg[i][j] = intensity; //intensity
            }
        }
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j)
                imageg << arrayg[i][j] << " " << arrayg[i][j] << " " << arrayg[i][j] << " ";
            imageg << std::endl;
        }
        imageg << "\n";
        imageg.close();

        int** maskX;
        maskX = new int* [height];
        for (int i = 0; i < height; i++)
            maskX[i] = new int[width];

        int** maskY;
        maskY = new int* [height];
        for (int i = 0; i < height; i++)
            maskY[i] = new int[width];

        maskX = maskx(arrayg);
        maskY = masky(arrayg);

        //Finding the intensity gradient of the image

        int** magnitude;
        magnitude = new int* [height];
        for (int i = 0; i < height; i++)
            magnitude[i] = new int[width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                magnitude[i][j] = (int)(sqrt((maskX[i][j] * maskX[i][j]) + (maskY[i][j] * maskY[i][j])));

        /*for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                std::cout << magnitude[i][j] << " ";
            std::cout << "\n";
        }
        std::cout << "\n";*/

        // apply double threshold
        int** threshold;
        threshold = new int* [height];
        for (int i = 0; i < height; i++)
            threshold[i] = new int[width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                if (magnitude[i][j] < 10)
                    threshold[i][j] = 0;
                else if (magnitude[i][j] < 120)
                    threshold[i][j] = 1; // weak edge
                else
                    threshold[i][j] = 2; // strong edge
            }

        // Hysteresis

        // any 1 connected to a 2 through 1s or 2s will be promoted to a 2

        threshold = hysteresis(threshold); //returns with 2s 1s and 0s

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                if (threshold[i][j] == 2)
                    threshold[i][j] = 0;
                else
                    threshold[i][j] = 1;
            }

        /*int** direction;
        direction = new int* [height];
        for (int i = 0; i < height; i++)
            direction[i] = new int[width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                direction[i][j] = atan2(maskY[i][j], maskX[i][j]) * 180 / M_PI;
        */

        //Gradient magnitude thresholding or lower bound cut-off suppression

        //if the rounded gradient angle is 0° (edge is in the north–south direction) -> if its gradient magnitude is greater than the magnitudes at pixels in the east and west directions

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (threshold[i][j] == 0)
                    imagem << 255 << " " << 255 << " " << 255 << " ";
                else
                    imagem << 0 << " " << 0 << " " << 0 << " ";
            }
            imagem << std::endl;
        }
        imagem << "\n";
        imagem.close();

        og.close();
    }
    else
        std::cout << "Unable to open file";
}

void part3(int low, int high, std::string filename) {
    //std::cout << low << " " << high << " " << filename << "\n";
    std::ifstream og("image.ppm");
    
    std::string suffix = ".ppm";
        if(filename.size() >= suffix.size() && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0)
            std::ifstream og(filename);

    if (low == 0 && high == 0){
        low = 20;
        high = 120;
    }

    // command prompt: ./l053 -L 110 -H 200 -F myimage.ppm
    
    std::string next;
    if (og.is_open()) {
        og >> next;
        og >> next;
        width = std::stoi(next);
        og >> next;
        height = std::stoi(next);
        og >> next;
        int maxcolor = std::stoi(next);
        //og >> next;

        /*int** ogarray;
        ogarray = new int* [height];
        for (int i = 0; i < height; i++)
            ogarray[i] = new int[width];*/

        std::ofstream imageg;
        imageg.open("imageg.ppm");
        imageg << "P3 " << width << " " << height << " " << maxcolor << "\n";
        // make array
        int** arrayg;
        arrayg = new int* [height];
        for (int i = 0; i < height; i++)
            arrayg[i] = new int[width];

        /*std::ofstream imagem;
        imagem.open("imagem.ppm");
        imagem << "P3 " << width << " " << height << " 255\n";
        // make array
        int** arraym;
        arraym = new int* [height];
        for (int i = 0; i < height; i++)
            arraym[i] = new int[width];*/

        int r;
        int g;
        int b;
        int intensity = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                og >> next;
                r = std::stoi(next);
                og >> next;
                g = std::stoi(next);
                og >> next;
                b = std::stoi(next);
                intensity = (int)((r + g + b) / 3);
                arrayg[i][j] = intensity; //intensity
            }
        }
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j)
                imageg << arrayg[i][j] << " " << arrayg[i][j] << " " << arrayg[i][j] << " ";
            imageg << std::endl;
        }
        imageg << "\n";
        imageg.close();

        int** maskX;
        maskX = new int* [height];
        for (int i = 0; i < height; i++)
            maskX[i] = new int[width];

        int** maskY;
        maskY = new int* [height];
        for (int i = 0; i < height; i++)
            maskY[i] = new int[width];

        maskX = maskx(arrayg);
        maskY = masky(arrayg);

        //Finding the intensity gradient of the image

        int** magnitude;
        magnitude = new int* [height];
        for (int i = 0; i < height; i++)
            magnitude[i] = new int[width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                magnitude[i][j] = (int)(sqrt((maskX[i][j] * maskX[i][j]) + (maskY[i][j] * maskY[i][j])));

        /*for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                std::cout << magnitude[i][j] << " ";
            std::cout << "\n";
        }
        std::cout << "\n";*/

        // apply double threshold
        int** threshold;
        threshold = new int* [height];
        for (int i = 0; i < height; i++)
            threshold[i] = new int[width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                if (magnitude[i][j] < low) //20
                    threshold[i][j] = 0;
                else if (magnitude[i][j] < high) //120
                    threshold[i][j] = 1; // weak edge
                else
                    threshold[i][j] = 2; // strong edge
            }

        // Hysteresis

        // any 1 connected to a 2 through 1s or 2s will be promoted to a 2

        threshold = hysteresis(threshold); //returns with 2s 1s and 0s

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                if (threshold[i][j] == 2)
                    threshold[i][j] = 1;
                else
                    threshold[i][j] = 0;
            }

        std::ofstream image1;
        image1.open("image1.ppm");
        image1 << "P3 " << width << " " << height << " 1\n";

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (threshold[i][j] == 1)
                    image1 << 1 << " " << 1 << " " << 1 << " ";
                else
                    image1 << 0 << " " << 0 << " " << 0 << " ";
            }
            image1 << std::endl;
        }
        image1 << "\n";
        image1.close();

        int** direction;
        direction = new int* [height];
        for (int i = 0; i < height; i++)
            direction[i] = new int[width];

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                direction[i][j] = atan2(maskY[i][j], maskX[i][j]) * 180 / M_PI;

        //if the rounded gradient angle is 0° (edge is in the north–south direction) -> if its gradient magnitude is greater than the magnitudes at pixels in the east and west directions
        int** roundedangle;
        roundedangle = new int* [height];
        for (int i = 0; i < height; i++)
            roundedangle[i] = new int[width];

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (direction[i][j] < 22.5)
                    roundedangle[i][j] = 0;
                else if (direction[i][j] < 67.5)
                    roundedangle[i][j] = 45;
                else if (direction[i][j] < 112.5)
                    roundedangle[i][j] = 90;
                else if (direction[i][j] < 157.5)
                    roundedangle[i][j] = 135;
                else
                    roundedangle[i][j] = 0;
            }
        }

        /*for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                std::cout << roundedangle[i][j] << " ";
            std::cout << "\n";
        }
        std::cout << "\n";*/

        //compare magnitude on both sides and if the point is bigger than both, set to 1, otherwise, 0.
        int** nms;
        nms = new int* [height];
        for (int i = 0; i < height; i++)
            nms[i] = new int[width];

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (roundedangle[i][j] == 0) {
                    if (i > 0 && i < height - 1) {
                        if (magnitude[i][j] >= magnitude[i - 1][j] && magnitude[i][j] >= magnitude[i + 1][j])
                            nms[i][j] = 1;
                        else
                            nms[i][j] = 0;
                    }
                }
                else if (roundedangle[i][j] == 45) {
                    if (i > 0 && i < height - 1 && j > 0 && j < width - 1) {
                        if (magnitude[i][j] >= magnitude[i + 1][j - 1] && magnitude[i][j] >= magnitude[i - 1][j + 1])
                            nms[i][j] = 1;
                        else
                            nms[i][j] = 0;
                    }
                }
                else if (roundedangle[i][j] == 90) {
                    if (j > 0 && j < width - 1) {
                        if (magnitude[i][j] >= magnitude[i][j - 1] && magnitude[i][j] >= magnitude[i][j + 1])
                            nms[i][j] = 1;
                        else
                            nms[i][j] = 0;
                    }
                }
                else{// if (roundedangle[i][j] == 135) {
                    if (i > 0 && i < height - 1 && j > 0 && j < width - 1) {
                        if (magnitude[i][j] >= magnitude[i - 1][j - 1] && magnitude[i][j] >= magnitude[i + 1][j + 1])
                            nms[i][j] = 1;
                        else
                            nms[i][j] = 0;
                    }
                }
            }
        }

        std::ofstream image2;
        image2.open("image2.ppm");
        image2 << "P3 " << width << " " << height << " 1\n";

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (nms[i][j] == 1)
                    image2 << 1 << " " << 1 << " " << 1 << " ";
                else
                    image2 << 0 << " " << 0 << " " << 0 << " ";
            }
            image2 << std::endl;
        }
        image2 << "\n";
        image2.close();

        int** edges;
        edges = new int* [height];
        for (int i = 0; i < height; i++)
            edges[i] = new int[width];

        //nms and threshold
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (nms[i][j] == 1 && threshold[i][j] == 1)
                    edges[i][j] = 1;
                else
                    edges[i][j] = 0;
            }
        }

        std::ofstream imagef;
        imagef.open("imagef.ppm");
        imagef << "P3 " << width << " " << height << " 1\n";

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (edges[i][j] == 1)
                    imagef << 1 << " " << 1 << " " << 1 << " ";
                else
                    imagef << 0 << " " << 0 << " " << 0 << " ";
            }
            imagef << std::endl;
        }
        imagef << "\n";
        imagef.close();

        og.close();
    }
    else
        std::cout << "Unable to open file";
}

int main(int argc, char* argv[]) {
    //part1();
    //part2();
    part3(atoi(argv[2]), atoi(argv[4]), std::string(argv[6]));
}
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <filesystem>

int main() {
    const int minPower = 10;
    const int maxPower = 20;
    const int numFilesPerSize = 100;
    const int maxValue = 1100000;

    // Create output directory
    std::filesystem::create_directory("output");

    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, maxValue);

    for (int power = minPower; power <= maxPower; ++power) {
        int listSize = 1 << power;

        for (int fileNum = 0; fileNum < numFilesPerSize; ++fileNum) {
            std::string filename = "output/data_" + std::to_string(power) + "_" + std::to_string(fileNum) + ".txt";
            std::ofstream outFile(filename);

            if (!outFile) {
                std::cerr << "Failed to create file: " << filename << std::endl;
                continue;
            }

            for (int i = 0; i < listSize; ++i) {
                outFile << distrib(gen) << std::endl;;
                if (i != listSize - 1) outFile << " ";
            }

            outFile.close();
            std::cout << "Generated: " << filename << std::endl;
        }
    }

    std::cout << "All files generated successfully!" << std::endl;
    return 0;
}

#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>
#include <fstream>

using namespace std;

int main() {
    // Seed the random number generator
    srand(time(0));
    ofstream file;
    string filename = " ";
 

    int power;
    cout << "Enter the power of 2 for the array size: ";
    cin >> power;
    cout << "Enter filename: ";
    cin >> filename;
    

    int size = 1 << power; // Equivalent to pow(2, power)

    // Allocate array dynamically
    int* arr = new int[size];

    file.open(filename);
    for (int i = 0; i < size; i++) {
        arr[i] = (rand() % 1000000) + 1;
        file << arr[i] << endl;
    }
    file.close();
    delete[] arr;
    cout << "success!" << endl;

    return 0;
}

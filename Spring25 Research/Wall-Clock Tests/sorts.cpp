#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

void heapify(int arr[], int n, int i)
{
    int largest = i; // Initialize largest as root Since we are using 0 based indexing
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2
 
    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;
 
    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;
 
    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);
 
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}
 
// main function to do heap sort
void heapSort(int arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
 
    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {
        // Move current root to end
        swap(arr[0], arr[i]);
 
        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

int partition(vector<int> &vec, int low, int high) {

    // Selecting last element as the pivot
    int pivot = vec[high];

    // Index of elemment just before the last element
    // It is used for swapping
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {

        // If current element is smaller than or
        // equal to pivot
        if (vec[j] <= pivot) {
            i++;
            swap(vec[i], vec[j]);
        }
    }

    // Put pivot to its position
    swap(vec[i + 1], vec[high]);

    // Return the point of partition
    return (i + 1);
}

void quickSort(vector<int> &vec, int low, int high) {

    // Base case: This part will be executed till the starting
    // index low is lesser than the ending index high
    if (low < high) {

        // pi is Partitioning Index, arr[p] is now at
        // right place
        int pi = partition(vec, low, high);

        // Separately sort elements before and after the
        // Partition Index pi
        quickSort(vec, low, pi - 1);
        quickSort(vec, pi + 1, high);
    }
}

void merge(vector<int>& arr, int left, 
    int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temp vectors
    vector<int> L(n1), R(n2);

    // Copy data to temp vectors L[] and R[]
    for (int i = 0; i < n1; i++)
    L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    // Merge the temp vectors back 
    // into arr[left..right]
    while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
    arr[k] = L[i];
    i++;
    }
    else {
    arr[k] = R[j];
    j++;
    }
    k++;
    }

    // Copy the remaining elements of L[], 
    // if there are any
    while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
    }

    // Copy the remaining elements of R[], 
    // if there are any
    while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
}
}

// begin is for left index and end is right index
// of the sub-array of arr to be sorted
void mergeSort(vector<int>& arr, int left, int right)
{
    if (left >= right)
    return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int main() {
    int power;
    cout << "Enter the power for the dataset files (e.g., 10 for 2^10): ";
    cin >> power;

    vector<long long> stdDurations, mergeDurations, quickDurations, heapDurations;
    string basePath = "data/";
    int temp;

    for (int fileNum = 0; fileNum < 100; ++fileNum) {
        vector<int> sortVec, mergeVec, quickVec;
        string filename = basePath + "data_" + to_string(power) + "_" + to_string(fileNum) + ".txt";
        ifstream input(filename);

        if (!input.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            continue;
        }

        while (input >> temp) {
            sortVec.push_back(temp);
            mergeVec.push_back(temp);
            quickVec.push_back(temp);
        }
        input.close();

        int n = sortVec.size();
        int* heapArr = new int[n];
        for (int i = 0; i < n; ++i)
            heapArr[i] = sortVec[i];

        // std::sort
        auto start = chrono::high_resolution_clock::now();
        sort(sortVec.begin(), sortVec.end());
        auto end = chrono::high_resolution_clock::now();
        stdDurations.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());

        // Merge Sort
        start = chrono::high_resolution_clock::now();
        mergeSort(mergeVec, 0, mergeVec.size() - 1);
        end = chrono::high_resolution_clock::now();
        mergeDurations.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());

        // Quick Sort
        start = chrono::high_resolution_clock::now();
        quickSort(quickVec, 0, quickVec.size() - 1);
        end = chrono::high_resolution_clock::now();
        quickDurations.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());

        // Heap Sort
        start = chrono::high_resolution_clock::now();
        for (int i = n / 2 - 1; i >= 0; --i)
            heapify(heapArr, n, i);
        heapSort(heapArr, n);
        end = chrono::high_resolution_clock::now();
        heapDurations.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());

        delete[] heapArr;

        cout << "Finished file #" << fileNum << endl;
    }

    auto avg = [](const vector<long long>& durations) {
        long long total = 0;
        for (auto d : durations) total += d;
        return durations.empty() ? 0 : total / durations.size();
    };

    cout << "\nAverage execution times (microseconds) over 100 files of size 2^" << power << ":\n";
    cout << "std::sort:    " << avg(stdDurations) << " microseconds\n";
    cout << "Merge Sort:   " << avg(mergeDurations) << " microseconds\n";
    cout << "Quick Sort:   " << avg(quickDurations) << " microseconds\n";
    cout << "Heap Sort:    " << avg(heapDurations) << " microseconds\n";

    return 0;
}
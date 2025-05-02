#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

class OnlineSelectionSearch {
    public:
        OnlineSelectionSearch(std::vector<int>& A) : A(A), n(A.size()), V(n, 0) {
            preprocess();
        }
    
       int select(int s) {
            if (V[s] == 1) return A[s];
            
            int a = s, b = s;
            while (a >= 0 && V[a] != 1) a--;
            while (b < n && V[b] != 1) b++;
            
            return qselect(a + 1, b - 1, s);
        }
    
        int search(int p) {
            if (p < A[0]) return 0;
            if (p >= A[n - 1]) return n;
    
            int a = 1, b = n;
            while (a + 1 < b) {
                int mid = (a + b) / 2;
                if (p < A[mid - 1]){ 
                    b = mid;
                }
                else if (p > A[mid - 1]){ 
                    a = mid;
                }
                else {                
                    a = b = mid; break; 
                }
            }
            
            while (a > 0 && V[a] != 1) a--;
            while (b < n && V[b] != 1) b++;
            
            return qsearch(a + 1, b - 1, p);
        }
    
        bool checkCorrectness(std::vector<int>& test) {
    
            for (int i = 1; i < n; i++) {
                if (A[i] != test[i]) {
                    //std::cout << "Incorrect!" << std::endl;
                    return false;
                }
            }
            //std::cout << "Correct!" << std::endl;
            return true;
        }
    
        int getA(int i){
            return A[i];
        }
        int getV(int i){
            return V[i];
        }
        void setA(int i, int val){
            A[i] = val;
        }
        void setV(int i, int val){
            V[i] = val;
        }
    
        void print(){
            cout << "A[";
            for(int i=0; i < A.size(); i++){
                cout << A[i] << " ";
            }
            cout << "]\n" << endl;
        }
    
        std::vector<int> getArr(){
            return A;
        }
    
    private:
        std::vector<int> A;
        int n;
        std::vector<int> V;
        
    
        void preprocess() {
            int min=A[0];
            int max=A[0];
            int in1=0, in2=0;
    
            for(int i=1; i<n; i++){
                if(min > A[i]){
                    min=A[i];
                    in1=i;
                }
    
                if(max < A[i]){
                    max = A[i];
                    in2=i;
                }
            }
    
            std::swap(A[0], A[in1]);
            std::swap(A[n - 1], A[in2]);
            V[0] = V[n - 1] = 1;
        }
    
        int findMedian(std::vector<int>& arr, int start, int end) {
            int mid = start + (end - start) / 2;
            std::nth_element(arr.begin() + start, arr.begin() + mid, arr.begin() + end + 1);
            return arr[mid];
        }
    
        int mediansOfMedians(int l, int r) {
            if (r - l + 1 <= 5) {
                return findMedian(A, l, r);
            }
        
            std::vector<int> medians;
            for (int i = l; i <= r; i += 5) {
                int subEnd = std::min(i + 4, r);
                medians.push_back(findMedian(A, i, subEnd));
            }
        
            return findMedian(medians, 0, medians.size() - 1);
        }
    
        int partition(int l, int r, int p) {
            int i = l;
            for (int j = l; j < r; j++) {
                if (A[j] < p) {
                    std::swap(A[i], A[j]);
                    i++;
                }
            }
            std::swap(A[i], A[r]);
    
            return i;
        }
    
        int qselect(int l, int r, int s) {
    
            if (l == r) {
                V[s] = 1;
                return A[s];
            }
    
            int pivot = A[r];
    
            int pivotpos = partition(l, r, pivot);
            V[pivotpos] = 1;
    
            if (s == pivotpos) return A[pivotpos];
            else if (s < pivotpos) return qselect(l, pivotpos - 1, s);
            else return qselect(pivotpos + 1, r, s);
        }
    
        int qsearch(int l, int r, int p) {
            int rank = l;
            if (l >= r) {
                if (p > A[rank]) rank++;
                V[rank] = 1;
                return rank;
            }
    
            int pivotpos = partition(l, r, p);
            V[pivotpos] = 1;
    
            if (p == A[pivotpos]) return pivotpos;
            else if (p < A[pivotpos]) return qsearch(l, pivotpos - 1, p);
            else return qsearch(pivotpos + 1, r, p);
        }
};

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
    /*int power;
    cout << "Enter the power for the dataset files (e.g., 10 for 2^10): ";
    cin >> power;*/

    vector<long long> stdDurations, mergeDurations, quickDurations, heapDurations, simpleDurations;
    string basePath = "data/";
    int temp;

    for(int power=10; power <= 20; ++power) {
        vector<long long> stdDurations, mergeDurations, quickDurations, heapDurations, simpleDurations;

        for (int fileNum = 0; fileNum < 100; ++fileNum) {
            vector<int> sortVec, mergeVec, quickVec, array;
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
                array.push_back(temp);
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

            OnlineSelectionSearch oss(array);

            start = chrono::high_resolution_clock::now();
            for (int i = 0; i < array.size(); i += 2) {
                oss.select(i);
            }
            int counter=0;

            for(int i=0; i<array.size(); i++){
                if(oss.getV(i) == 0){
                    counter++;
                }
                else{
                    if(counter ==2){
                        if(oss.getA(i-2) > oss.getA(i - 1)){
                            int temp = oss.getA(i - 2);
                            oss.setA(i - 2, oss.getA(i - 1));
                            oss.setA(i - 1, temp);
                        }
                    }
                    else if(counter == 3){
                        if (oss.getA(i - 3) > oss.getA(i - 2)) {
                            int temp = oss.getA(i - 3);
                            oss.setA(i - 3, oss.getA(i - 2));
                            oss.setA(i - 2, temp);
                        }
                        if (oss.getA(i - 2) > oss.getA(i - 1)) {
                            int temp = oss.getA(i - 2);
                            oss.setA(i - 2, oss.getA(i - 1));
                            oss.setA(i - 1, temp);
                        }
                        if (oss.getA(i - 3) > oss.getA(i - 2)) {
                            int temp = oss.getA(i - 3);
                            oss.setA(i - 3, oss.getA(i - 2));
                            oss.setA(i - 2, temp);
                        }
                    }
                    counter=0;
                }
            }
            end = chrono::high_resolution_clock::now();
            simpleDurations.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());

            //cout << "Finished file #" << fileNum << endl;
        }

        long long total = 0;
        for (long long d : simpleDurations) {
            total += d;
        }
        cout << "\nAverage execution times (microseconds) over 100 files of size 2^" << power << ":\n";
        cout << "Simple Sort:  " << (total / simpleDurations.size()) << " microseconds\n";

        total = 0;
        for (long long d : stdDurations) {
            total += d;
        }
        cout << "std::sort:    " << (total / stdDurations.size()) << " microseconds\n";

        total = 0;
        for (long long d : mergeDurations) {
            total += d;
        }
        cout << "Merge Sort:   " << (total / mergeDurations.size()) << " microseconds\n";

        total = 0;
        for (long long d : quickDurations) {
            total += d;
        }
        cout << "Quick Sort:   " << (total / quickDurations.size()) << " microseconds\n";

        total = 0;
        for (long long d : heapDurations) {
            total += d;
        }
        cout << "Heap Sort:    " << (total / heapDurations.size()) << " microseconds\n\n";

    }
    return 0;
}
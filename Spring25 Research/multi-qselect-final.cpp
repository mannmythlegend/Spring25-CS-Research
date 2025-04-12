#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
using namespace std;

class OnlineSelectionSearch {
public:
    OnlineSelectionSearch(std::vector<int>& A) : A(A), n(A.size()), V(n, 0) {
        preprocess();
    }


    // base select
    // calls  qselect for the recursive work
   int select(int s) {
        if (V[s - 1] == 1) return A[s - 1];
        
        int a = s - 1, b = s - 1;
        while (a >= 0 && V[a] != 1) a--;
        while (b < n && V[b] != 1) b++;
        
        return qselect(a + 1, b - 1, s - 1);
    }

    // search function
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

    // testing correctness of sorting
    void checkCorrectness(std::vector<int>& test) {

        for (int i = 1; i < n; i++) {
            if (A[i] != test[i]) {
                std::cout << "Incorrect!" << std::endl;
                return;
            }
        }
        std::cout << "Correct!" << std::endl;
    }

    // getters and setters for A and V
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
    
    // preprocess to find min and max.
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

    // find median if used with mediansOfMedians
    int findMedian(std::vector<int>& arr, int start, int end) {
        int mid = start + (end - start) / 2;
        std::nth_element(arr.begin() + start, arr.begin() + mid, arr.begin() + end + 1);
        return arr[mid];
    }

    // medians of medians to find "best pivot (slow)"
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

    // partition
    // Lomuto partitioning scheme (in-place partitioning)
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
        // taking A[r] is very compatible with Lomuto
        // If wanted to use mediansOfMedians, uncomment the next line
        //int pivot = mediansOfMedians(l, r);
        int pivot = A[r];

        int pivotpos = partition(l, r, pivot);
        V[pivotpos] = 1; // mark pivot as visited
        
        // continue qselect
        if (s == pivotpos) return A[pivotpos];
        else if (s < pivotpos) return qselect(l, pivotpos - 1, s);
        else return qselect(pivotpos + 1, r, s);
    }

    // qsearch for search function
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

int main() {
    std::vector<int> array;
    ifstream input;
    vector<int> test;
    int temp =0;
    string filename = " ";
    cout << "Enter the filename you want to read the array from: ";
    cin >> filename;
    input.open(filename);

    // file i/o
    if(!input.is_open()){
        cout << "error opening file." << endl;
        return 1;
    }
    else{
        while(input >> temp){
            array.push_back(temp);
            test.push_back(temp);
        }
    }
    
    // testing correctness of sorting with our algo
    std::sort(test.begin(), test.end());

    vector<int> s;


    OnlineSelectionSearch oss(array);

    // selects every 3rd element of the vector without sorting
    // and sorts the 2 elements in between to ensure correctness
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<array.size(); i+=3){
        oss.select(i+1);
        if(i>0){
            if(oss.getA(i-1) < oss.getA(i-2)){
                int temp=oss.getA(i-1);
                oss.setA(i-1, oss.getA(i-2));
                oss.setA(i-2, temp);
                
                oss.setV(i-1, 1);
                oss.setV(i-2, 1);
            }
        }
        s.push_back(oss.getA(i));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Execution time: " << duration.count() << " microseconds\n" <<  endl;


    // test with our sorted vector
    cout << "The Answer: ";
    oss.checkCorrectness(test);
    cout << endl;

    cout << "Would you like to see all of the selections? (y/n): ";
    char choice;
    cin >> choice;
    if(choice == 'y'){
        cout << "The selections are: ";
        for(int i=0; i<s.size(); i++){
            cout << s[i] << " ";
        }
        cout << endl;
    }
    else if(choice == 'n'){
        cout << "Okay, exiting." << endl;
    }
    else{
        cout << "Invalid input." << endl;
    }

    return 0;
}
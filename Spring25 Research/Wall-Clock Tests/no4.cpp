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

   int select(int s) {
        if (V[s - 1] == 1) return A[s - 1];
        
        int a = s - 1, b = s - 1;
        while (a >= 0 && V[a] != 1) a--;
        while (b < n && V[b] != 1) b++;
        
        return qselect(a + 1, b - 1, s - 1);
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
                std::cout << "Incorrect!" << std::endl;
                return false;
            }
        }
        std::cout << "Correct!" << std::endl;
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

int main() {
    vector<long long> durations;
    vector<int> array, test;
    vector<bool> isCorrect;
    bool tempBool = true;
    int temp = 0;

    int power;
    cout << "Enter the power for the dataset files (e.g., 10 for 2^10): ";
    cin >> power;

    string basePath = "data/";
    for (int fileNum = 0; fileNum < 100; ++fileNum) {
        string filename = basePath + "data_" + to_string(power) + "_" + to_string(fileNum) + ".txt";
        ifstream input(filename);
        if (!input.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            continue;
        }

        array.clear();
        test.clear();
        while (input >> temp) {
            array.push_back(temp);
            test.push_back(temp);
        }
        input.close();

        sort(test.begin(), test.end());

        OnlineSelectionSearch oss(array);

        auto start = chrono::high_resolution_clock::now();
        for(int i=0; i<array.size(); i+=4){
            if(i!=array.size()){
                oss.select(i+1);
            }
            if(i>0){
    
                if(oss.getA(i-3) > oss.getA(i-2)){
                    int temp = oss.getA(i-3);
                    oss.setA(i-3, oss.getA(i-2));
                    oss.setA(i-2, temp);
                }
                if(oss.getA(i-2) > oss.getA(i-1)){
                    temp = oss.getA(i-2);
                    oss.setA(i-2, oss.getA(i-1));
                    oss.setA(i-1, temp);
                }
                if(oss.getA(i-3) > oss.getA(i-2)){
                    int temp = oss.getA(i-3);
                    oss.setA(i-3, oss.getA(i-2));
                    oss.setA(i-2, temp);
                }
                oss.setV(i-3, 1);
                oss.setV(i-2, 1);
                oss.setV(i-1, 1);
            }
        }
        if(oss.getA(array.size()-3) > oss.getA(array.size()-2)){
            int temp = oss.getA(array.size()-3);
            oss.setA(array.size()-3, oss.getA(array.size()-2));
            oss.setA(array.size()-2, temp);
        }
        if(oss.getA(array.size()-2) > oss.getA(array.size()-1)){
            temp = oss.getA(array.size()-2);
            oss.setA(array.size()-2, oss.getA(array.size()-1));
            oss.setA(array.size()-1, temp);
        }
        if(oss.getA(array.size()-3) > oss.getA(array.size()-2)){
            int temp = oss.getA(array.size()-3);
            oss.setA(array.size()-3, oss.getA(array.size()-2));
            oss.setA(array.size()-2, temp);
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        durations.push_back(duration.count());

        cout << "File #" << fileNum << " executed in " << duration.count() << " microseconds\n";
        cout << "Answer check: ";
        tempBool = oss.checkCorrectness(test);
        isCorrect.push_back(tempBool);
        cout << "\n" << endl;
    }

    long long total = 0;
    for (long long d : durations) {
        total += d;
    }
    cout << "Average execution time: " << (total / durations.size()) << " microseconds" << endl;

    int numCorrect=0;
    for(int i=0; i<isCorrect.size(); i++){
        if(!isCorrect[i]){
            cout << "File #" << i << " is incorrect." << endl;
        }
        else{
            numCorrect++;
        }
    }
    cout << numCorrect << " files are correct." << endl;

    return 0;
}
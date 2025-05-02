// //////////////////////////////////////////////////////////
// count.cpp
// Copyright (c) 2014,2020 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

// g++ -O3 -std=c++11 count.cpp -o count
// ./count [size]
// if [size] is omitted then 100 integers are sorted

#include <cstdio>
#include <fstream>
#include <iostream>

#include <vector>
#include <algorithm> // std::sort, std::reverse

#include "sort.h"
using namespace std;

/// count assignments and comparisions
class Number
{
public:
  /// set value
  Number(int x = 0) : value(x) { }
  /// copy constructor is counted as an assignment
  Number(const Number& other) : value(other.value) { ++numAssignments; }

  static int numLessThan;
  /// count comparisons
  bool operator< (const Number& other) const { ++numLessThan; return value <  other.value; }
  bool operator> (const Number& other) const { ++numLessThan; return value >  other.value; }
  bool operator<= (const Number& other) const { ++numLessThan; return value <=  other.value; }
  bool operator>= (const Number& other) const { ++numLessThan; return value >=  other.value; }

  static int numAssignments;
  /// count assignments
  void operator= (const Number& other)       { ++numAssignments;     value =  other.value; }

  bool operator==(const Number& other) const { return                value == other.value; }
  bool operator!=(const Number& other) const { return                value != other.value; }

  /// reset counters
  static void reset() { numLessThan = numAssignments = 0; }

private:
  /// actually just a simple integer
  int value;
};

class OnlineSelectionSearch {
    public:
        OnlineSelectionSearch(std::vector<Number>& A) : A(A), n(A.size()), V(n, 0) {
            preprocess();
        }
    
        Number select(int s) {
            if (V[s] == 1) return A[s];
            
            int a = s, b = s;
            while (a >= 0 && V[a] != 1) a--;
            while (b < n && V[b] != 1) b++;
            
            return qselect(a + 1, b - 1, s);
        }
    
        int search(Number p) {
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
        Number getA(int index){
            return A[index];
        }
        int getV(int index){
            return V[index];
        }

        void setV(int index, int value){
            V[index] = value;
        }
        void setA(int index, Number value){
            A[index] = value;
        }
    
    private:
        std::vector<Number> A;
        int n;
        std::vector<int> V;
    
        void preprocess() {
            int in1 = 0, in2 = 0;
            for (int i = 1; i < n; i++) {
                if (A[i] < A[in1]) in1 = i;
                if (A[i] > A[in2]) in2 = i;
            }
            std::swap(A[0], A[in1]);
            std::swap(A[n - 1], A[in2]);
            V[0] = V[n - 1] = 1;
        }
    
        Number findMedian(std::vector<Number>& arr, int start, int end) {
            int size = end - start + 1;
            std::vector<Number> temp(arr.begin() + start, arr.begin() + end + 1);
            std::sort(temp.begin(), temp.end());
            return temp[size / 2];
        }
    
        Number mediansOfMedians(int l, int r) {
            if (r - l + 1 <= 5) {
                return findMedian(A, l, r);
            }
            std::vector<Number> medians;
            for (int i = l; i <= r; i += 5) {
                int subEnd = std::min(i + 4, r);
                medians.push_back(findMedian(A, i, subEnd));
            }
            return findMedian(medians, 0, medians.size() - 1);
        }
    
        int partition(int l, int r, Number p) {
            /*int pivotIndex = l;
            while (A[pivotIndex] != p && pivotIndex <= r) pivotIndex++;
            std::swap(A[pivotIndex], A[r]);*/
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
    
        Number qselect(int l, int r, int s) {
            if (l == r) {
                V[s] = 1;
                return A[s];
            }
            //Number pivot = mediansOfMedians(l, r);
            Number pivot = A[r];
            int pivotpos = partition(l, r, pivot);
            V[pivotpos] = 1;
            if (s == pivotpos) return A[pivotpos];
            else if (s < pivotpos) return qselect(l, pivotpos - 1, s);
            else return qselect(pivotpos + 1, r, s);
        }
    
        int qsearch(int l, int r, Number p) {
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

// statics
int Number::numLessThan    = 0;
int Number::numAssignments = 0;

// array to be sorted
typedef std::vector<Number> Container;


// protect server from overload:
const int MaxSort = 2000000;


int main(int argc, char** argv)
{
    // number of elements to be sorted
    int n=0;
    

    

    string basePath = "data/";

    for(int power=10; power<=20; power++){
        vector<long long> comps;
        vector<long long> quick;
        vector<long long> merge;
        vector<long long> heap;
        vector<long long> stdSort;
        n=1 << power;
        Container data(n);
        for(int i=0; i<100; i++){
            string filename = basePath + "data_" + to_string(power) + "_" + to_string(i) + ".txt";
            ifstream input(filename);
            if (!input.is_open()) {
                cerr << "Error opening file: " << filename << endl;
                continue;
            }

            Container tempVec(n);

            for (int j = 0; j < n; j++) {
                int temp;
                input >> temp;
                tempVec[j] = Number(temp);
            }
            input.close();

            // heap sort
            data = tempVec;
            Number::reset();
            heapSort(data.begin(), data.end());
            heap.push_back(Number::numLessThan);
            //printf("\t%d", Number::numLessThan);

            // merge sort

            data = tempVec;
            Number::reset();
            mergeSort(data.begin(), data.end());
            merge.push_back(Number::numLessThan);
            //printf("\t%d", Number::numLessThan);

            data = tempVec;
            Number::reset();
            quickSort(data.begin(), data.end());
            quick.push_back(Number::numLessThan);
            //printf("\t%d", Number::numLessThan);

            // std::sort

            data = tempVec;
            Number::reset();
            std::sort(data.begin(), data.end());

            stdSort.push_back(Number::numLessThan);
            //printf("\t%d", Number::numLessThan);


            // simple algo
            OnlineSelectionSearch oss(tempVec);

            Number::reset();
            for (int i = 0; i < tempVec.size(); i += 4) {
                oss.select(i);
            }
            int counter = 0;
            for (int i = 0; i < tempVec.size(); i++) {
                if (oss.getV(i) == 0) {
                    counter++;
                } else {
                        if (counter == 2) {
                            if(oss.getA(i-2) > oss.getA(i - 1)){
                                Number temp = oss.getA(i - 2);
                                oss.setA(i - 2, oss.getA(i - 1));
                                oss.setA(i - 1, temp);
                            }
                        } else if (counter == 3) {
                            if (oss.getA(i - 3) > oss.getA(i - 2)) {
                                Number temp = oss.getA(i - 3);
                                oss.setA(i - 3, oss.getA(i - 2));
                                oss.setA(i - 2, temp);
                            }
                            if (oss.getA(i - 2) > oss.getA(i - 1)) {
                                Number temp = oss.getA(i - 2);
                                oss.setA(i - 2, oss.getA(i - 1));
                                oss.setA(i - 1, temp);
                            }
                            if (oss.getA(i - 3) > oss.getA(i - 2)) {
                                Number temp = oss.getA(i - 3);
                                oss.setA(i - 3, oss.getA(i - 2));
                                oss.setA(i - 2, temp);
                            }
                        }
                        counter = 0;
                    }
                }
                comps.push_back(Number::numLessThan);
                data.clear();
                tempVec.clear();
                Number::reset();

                //cout << "File #" << i << " has finished" << endl;
        }
        cout << "\n\nComparisons for " << power << ":" << endl;
        long long total = 0;
        for (long long d : comps) {
            total += d;
        }
        cout << "Simple \t" << total / comps.size() << endl;

        total = 0;
        for (long long d : stdSort) {
            total += d;
        }
        cout << "std::sort \t" << total / stdSort.size() << endl; 

        total = 0;
        for (long long d : merge) {
            total += d;
        }
        cout << "Merge \t" << total / merge.size() << endl;

        total = 0;
        for (long long d : quick) {
            total += d;
        }
        cout << "Quick \t" << total / quick.size() << endl;
        
        total = 0;
        for (long long d : heap) {
            total += d;
        }
        cout << "Heap \t" << total / heap.size() << endl;
    }
    return 0;
}
# Spring25-CS-Research

My research project makes multiselect more optimal. It creates a shadow bitvector (V), to mark a 1 when an element has been "visited" in the multiselection process. This allows the algorithm to take shortcuts, and become much after as it goes. 

We tested our algorithm against sorting algorithms (Mergesort, Quicksort, Heap Sort, std::sort()), and found that our multiselect algorithm can select n/3 queries, and sort the array faster than MergeSort, std::sort(), and sometimes heap sort in terms of Wall-Clock time!

Future implementation could include implementing our bitvector idea with Kanela Kaligosi's Research paper "Towards Optimal Online MultiSelect."

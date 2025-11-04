//
// Created by Manju Muralidharan on 10/19/25.
//

#ifndef HEAP_H
#define HEAP_H

#include <iostream>
using namespace std;

struct MinHeap {
    int data[64];
    int size;

    MinHeap() { size = 0; }

    void push(int idx, int weightArr[]) {

        data[size++] = idx;
        upheap(size-1, weightArr);
    }

    int pop(int weightArr[]) {

        if (size == 0) { // empty check
            return -1;
        }
        int out = data[0];
        data[0] = data[size-1];
        size--;
        if (size > 0 ) {
            downheap(0, weightArr);
        }

        return out;
    }

    void upheap(int pos, int weightArr[]) {

        while (pos > 0) {
            int parent = (pos -1) / 2;

            //Compare the weights
            if (weightArr[data[pos]] >= weightArr[data[parent]]) {
                break;
            }

            // Swap
            int temp = data[pos];
            data[pos] = data[parent];
            data[parent] = temp;

            // Move up
            pos = parent;
        }
    }

    void downheap(int pos, int weightArr[]) {

        while (true) {
            int left = 2 * pos + 1;
            int right = 2 * pos + 2;
            int smallest = pos;

            // Find the smallest among pos, left, and right
            if (left < size && weightArr[data[left]] < weightArr[data[smallest]]) {
                smallest = left;
            }

            if (right < size && weightArr[data[right]] < weightArr[data[smallest]]) {
            smallest = right;
            }

            if (smallest == pos) {
                break; // Heap property satisfied
            }

            // Swap parent with the smallest child
            int temp = data[pos];
            data[pos] = data[smallest];
            data[smallest] = temp;

            // Continue down
            pos = smallest;
        }
    }
};

#endif
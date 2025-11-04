//
// Created by Manju Muralidharan on 10/19/25.
//

/*
 * Author: Cade Fair
 * Course: CS 240
 * Assignment: PA2
 * Date: November 3, 2025
 *
 * Description:
 * This program reads a text file (input.txt), analyzes character frequencies,
 * builds a min-heap to combine symbols, constructs an
 * encoding tree, and prints both the encoding table and the encoded message.
 *
 * Frequent letters get shorter bit codes to reduce total size.
 * Implements an array-based heap and an iterative traversal using std::stack.
 */
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include "heap.h"
using namespace std;

// Global arrays for node information
const int MAX_NODES = 64;
int weightArr[MAX_NODES];
int leftArr[MAX_NODES];
int rightArr[MAX_NODES];
char charArr[MAX_NODES];

// Function prototypes
void buildFrequencyTable(int freq[], const string& filename);
int createLeafNodes(int freq[]);
int buildEncodingTree(int nextFree);
void generateCodes(int root, string codes[]);
void encodeMessage(const string& filename, string codes[]);

int main() {
    int freq[26] = {0};

    // Step 1: Read file and count letter frequencies
    buildFrequencyTable(freq, "input.txt");

    // Step 2: Create leaf nodes for each character with nonzero frequency
    int nextFree = createLeafNodes(freq);

    // Step 3: Build encoding tree using your heap
    int root = buildEncodingTree(nextFree);

    // Step 4: Generate binary codes using an STL stack
    string codes[26];
    generateCodes(root, codes);

    // Step 5: Encode the message and print output
    encodeMessage("input.txt", codes);

    return 0;
}

/*------------------------------------------------------
    Function Definitions (Students will complete logic)
  ------------------------------------------------------*/

// Step 1: Read file and count frequencies

void buildFrequencyTable(int freq[], const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    char ch;
    while (file.get(ch)) {
        // Convert uppercase to lowercase
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';

        // Count only lowercase letters
        if (ch >= 'a' && ch <= 'z')
            freq[ch - 'a']++;
    }
    file.close();

    cout << "Frequency table built successfully.\n";
}

// Step 2: Create leaf nodes for each character
int createLeafNodes(int freq[]) {
    int nextFree = 0;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            charArr[nextFree] = 'a' + i;
            weightArr[nextFree] = freq[i];
            leftArr[nextFree] = -1;
            rightArr[nextFree] = -1;
            nextFree++;
        }
    }
    cout << "Created " << nextFree << " leaf nodes.\n";
    return nextFree;
}

// Step 3: Build the encoding tree using heap operations
// Purpose: This will combine the lowest-frequency nodes from the heap
//          to construct the variable-bit encoding tree.
int buildEncodingTree(int nextFree) {
    // 1. Create a MinHeap object.
    MinHeap h;
    // 2. Push all leaf node indices into the heap.
    for ( int i = 0; i < nextFree; ++i) {
        h.push(i, weightArr);
    }
    // Empty
    if (h.size == 0) return -1;

    // Single case
    if (h.size == 1) return h.pop(weightArr);
    // 3. While the heap size is greater than 1:
    //    - Pop two smallest nodes
    //    - Create a new parent node with combined weight
    //    - Set left/right pointers
    //    - Push new parent index back into the heap

    int cur = nextFree;

    while (h.size >= 2) {
        int a = h.pop(weightArr); // smallest
        int b = h.pop(weightArr); // Next smallest

        int parent = cur++;
        charArr[parent] = '\0';
        leftArr[parent] = a;
        rightArr[parent] = b;
        weightArr[parent] = weightArr[a] + weightArr[b];

        h.push(parent, weightArr);

    }
    // 4. Return the index of the last remaining node (root)
    return h.pop(weightArr);


}

// Step 4: Use an STL stack to generate codes
// Purpose: Will traverse the tree and assign codes to each leaf character.
void generateCodes(int root, string codes[]) {

    for (int i = 0; i < 26; i++) {
        codes[i].clear();
    }

    if (root < 0) return; //Nothing to do

    //Single Node tree: will assign 0 to the only symbol
    if (leftArr[root] == -1 && rightArr[root] == -1) {
        if (charArr[root] >= 'a' && charArr[root] <= 'z') {
            codes[charArr[root] - 'a'] = "0";
        }
        return;
    }

    struct Item { int node; string path; };
    stack<Item> st;
    st.push({root, ""});

    while (!st.empty()) {
        Item it = st.top(); st.pop();
        int node = it.node;
        int L = leftArr[node];
        int R = rightArr[node];

        // Leaf and record the path
        if (L == -1 && R == -1){
            char c = charArr[node];
            if (c >= 'a' && c <= 'z') {
                // If path empty
                codes[c - 'a'] = it.path.empty() ? string("0") : it.path;
            }
        } else {
            // Push right first so left is processed first
            if (R != -1) {
                st.push({R, it.path + "1"});
            }

            if (L != -1) {
                st.push({L, it.path + "0"});
            }
        }
    }
}

// Step 5: Print table and encoded message
void encodeMessage(const string& filename, string codes[]) {
    cout << "\nCharacter : Code\n";
    for (int i = 0; i < 26; ++i) {
        if (!codes[i].empty())
            cout << char('a' + i) << " : " << codes[i] << "\n";
    }

    cout << "\nEncoded message:\n";

    ifstream file(filename);
    char ch;
    while (file.get(ch)) {
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';
        if (ch >= 'a' && ch <= 'z')
            cout << codes[ch - 'a'];
    }
    cout << "\n";
    file.close();
}
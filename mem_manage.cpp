#include <iostream>     // Include the standard input/output stream for displaying results.
#include <vector>       // Include vector library to use dynamic arrays (vectors).
#include <unordered_map> // Include unordered_map to store page lookup in constant time.
#include <algorithm>    // Include algorithm library for functions like `min_element`.
#include <queue>        // Include queue for the FIFO page replacement.

using namespace std;

// Function for FIFO (First-In, First-Out) Page Replacement
int fifoPageReplacement(vector<int>& referenceString, int numberOfFrames) {
    queue<int> pageQueue;              // A queue to track the pages in memory (FIFO).
    unordered_map<int, bool> inFrame;  // A map to check if a page is currently in memory (frames).
    int pageFaults = 0;                // Initialize the counter for page faults.

    // Iterate through each page in the reference string
    for (int page : referenceString) {
        if (!inFrame[page]) {   // If the page is not in memory (page fault occurs)
            pageFaults++;       // Increment the page fault counter

            // If memory is full (i.e., number of pages in the queue equals numberOfFrames)
            if (pageQueue.size() == numberOfFrames) {
                int removedPage = pageQueue.front();  // Get the page at the front (oldest page).
                pageQueue.pop();                      // Remove it from the queue.
                inFrame[removedPage] = false;        // Mark this page as not in memory.
            }

            pageQueue.push(page);         // Add the current page to the queue (new page).
            inFrame[page] = true;         // Mark the current page as in memory.
        }
    }

    return pageFaults;  // Return the total number of page faults.
}

// Function for LRU (Least Recently Used) Page Replacement
int lruPageReplacement(vector<int>& referenceString, int numberOfFrames) {
    unordered_map<int, int> lastUsed;  // Map to store the last used time of each page.
    vector<int> frames;                // Vector to store the pages currently in memory.
    int pageFaults = 0;                 // Initialize the counter for page faults.
    int currentTime = 0;                // Counter to track the time of each access.

    // Iterate through each page in the reference string
    for (int page : referenceString) {
        currentTime++;  // Increment the time counter for each access.

        auto it = find(frames.begin(), frames.end(), page);  // Find if the page is in memory (frames).
        
        if (it == frames.end()) { // If page not found in memory (page fault occurs)
            pageFaults++;  // Increment the page fault counter

            // If memory is full (i.e., number of pages in memory equals numberOfFrames)
            if (frames.size() == numberOfFrames) {
                // Find the Least Recently Used (LRU) page by checking the last used times.
                int lruPage = *min_element(frames.begin(), frames.end(), [&](int a, int b) {
                    return lastUsed[a] < lastUsed[b];  // Compare last used times of pages.
                });
                
                // Remove the LRU page from the memory (frames).
                auto lruIt = find(frames.begin(), frames.end(), lruPage);
                frames.erase(lruIt);  // Erase the LRU page from memory.
            }

            frames.push_back(page);  // Add the new page to memory.
        }

        lastUsed[page] = currentTime;  // Update the last used time for the current page.
    }

    return pageFaults;  // Return the total number of page faults.
}

// Function for Optimal Page Replacement
int optimalPageReplacement(vector<int>& referenceString, int numberOfFrames) {
    vector<int> frames;   // Vector to store the pages currently in memory.
    int pageFaults = 0;    // Initialize the counter for page faults.

    // Iterate through each page in the reference string
    for (int i = 0; i < referenceString.size(); i++) {
        int page = referenceString[i];  // Get the current page.

        // Check if the page is already in memory
        auto it = find(frames.begin(), frames.end(), page);  
        
        if (it == frames.end()) { // If page not found in memory (page fault occurs)
            pageFaults++;  // Increment the page fault counter

            // If memory is full (i.e., number of pages in memory equals numberOfFrames)
            if (frames.size() == numberOfFrames) {
                int farthest = -1;    // Variable to store the farthest used page.
                int indexToReplace = -1;  // Index of the page to replace.

                // Check which page in memory will be used the farthest in the future.
                for (int j = 0; j < frames.size(); j++) {
                    int nextUse = -1;  // Variable to track the next use of the current page.

                    // Look ahead in the reference string to find the next use of the page in memory.
                    for (int k = i + 1; k < referenceString.size(); k++) {
                        if (frames[j] == referenceString[k]) {
                            nextUse = k;  // Found the next use of the page.
                            break;
                        }
                    }

                    // If a page is not used again, replace it.
                    if (nextUse == -1) {
                        indexToReplace = j;
                        break;
                    }

                    // If the next use is farther away, mark this page as the one to replace.
                    if (nextUse > farthest) {
                        farthest = nextUse;
                        indexToReplace = j;
                    }
                }

                frames[indexToReplace] = page;  // Replace the page at the selected index with the current page.
            } else {
                frames.push_back(page);  // If space is available, just add the new page.
            }
        }
    }

    return pageFaults;  // Return the total number of page faults.
}

int main() {
    int numberOfFrames, numberOfPages;  // Variables to store the number of frames and pages.
    
    cout << "Enter the number of frames: ";  // Prompt the user for the number of frames.
    cin >> numberOfFrames;  // Input the number of frames.

    cout << "Enter the number of pages in the reference string: ";  // Prompt for the number of pages.
    cin >> numberOfPages;  // Input the number of pages.

    vector<int> referenceString(numberOfPages);  // Create a vector to store the reference string.
    cout << "Enter the reference string:\n";  // Prompt the user to enter the reference string.
    for (int i = 0; i < numberOfPages; i++) {
        cin >> referenceString[i];  // Input the reference string values.
    }

    // FIFO
    int fifoFaults = fifoPageReplacement(referenceString, numberOfFrames);  // Call the FIFO function.
    cout << "\nFIFO Page Faults: " << fifoFaults;  // Output the number of page faults for FIFO.

    // LRU
    int lruFaults = lruPageReplacement(referenceString, numberOfFrames);  // Call the LRU function.
    cout << "\nLRU Page Faults: " << lruFaults;  // Output the number of page faults for LRU.

    // Optimal
    int optimalFaults = optimalPageReplacement(referenceString, numberOfFrames);  // Call the Optimal function.
    cout << "\nOptimal Page Faults: " << optimalFaults;  // Output the number of page faults for Optimal.

    return 0;  // Return 0 to indicate successful execution.
}


// LabParCalc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <ctime>
#include <chrono>

using namespace std;

void generateRandomArray(int* array, int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(1000000, 9999999);

    for (int i = 0; i < size; ++i) {
        array[i] = dis(gen);
    }
}

void radixSort(int* array, int size) {

    int maxElement = array[0];
    for (int i = 1; i < size; ++i) {
        if (array[i] > maxElement)
            maxElement = array[i];
    }

    int maxDigitCount = 0;
    while (maxElement != 0) {
        maxElement /= 10;
        maxDigitCount++;
    }

    int exp = 1;
    for (int digit = 0; digit < maxDigitCount; digit++) {
        int count[10] = { 0 };

        for (int i = 0; i < size; i++)
            count[(array[i] / exp) % 10]++;

        for (int i = 1; i < 10; i++)
            count[i] += count[i - 1];

        int* output = new int[size];
        for (int i = size - 1; i >= 0; i--) {
            output[count[(array[i] / exp) % 10] - 1] = array[i];
            count[(array[i] / exp) % 10]--;
        }

        for (int i = 0; i < size; i++)
            array[i] = output[i];

        delete[] output;
        exp *= 10;
    }
}

int main(int argc, char** argv) {
    const int SIZE = 100000000;
    int* randomArray = new int[SIZE];
   
    generateRandomArray(randomArray, SIZE);
    auto start = chrono::high_resolution_clock::now();
    radixSort(randomArray, SIZE);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Sorting time without multithreading for " << SIZE << " elements: " << duration.count() << " seconds\n";

   /* cout << "First 10 sorted elements:\n";
    for (int i = 0; i < 10; ++i) {
        cout << randomArray[i] << " ";
    }
    cout << endl;
*/
    delete[] randomArray;

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

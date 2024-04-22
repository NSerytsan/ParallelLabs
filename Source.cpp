#include <iostream>
#include <random>
#include <ctime>
#include <mpi.h>
#include <chrono>

using namespace std;

void generateRandomArray(int array[], int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(1000000, 9999999);

    for (int i = 0; i < size; ++i) {
        array[i] = dis(gen);
    }
}

void radixSort(int array[], int size) {
    

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
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Розраховуємо розмір кожного фрагмента даних
    int fragmentSize = SIZE / size;
    int* localArray = new int[fragmentSize];
   
    // Розсилаємо фрагменти даних кожному процесу
    MPI_Scatter(randomArray, fragmentSize, MPI_INT, localArray, fragmentSize, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Сортуємо фрагменти даних кожним процесом паралельно
    radixSort(localArray, fragmentSize);
    
    // Збираємо відсортовані фрагменти даних на процесі 0
    int* sortedArray = nullptr;
    if (rank == 0) {
        sortedArray = new int[SIZE];
    }
    MPI_Gather(localArray, fragmentSize, MPI_INT, sortedArray, fragmentSize, MPI_INT, 0, MPI_COMM_WORLD);
    
    // вивід результатів на процесі 0
    if (rank == 0) {
        

        for (int i = 0; i < size; ++i) {
            cout << "Process " << i << " is processing indices " << i * fragmentSize << " to " << (i + 1) * fragmentSize - 1 << endl;
        }
        // Вивід перших 10 відсортованих елементів
      /*  cout << "First 10 sorted elements:\n";
        for (int i = 0; i < 10; ++i) {
            cout << sortedArray[i] << " ";
        }
        cout << endl;*/

        delete[] sortedArray;
    }

    delete[] randomArray;
    delete[] localArray;

    MPI_Finalize();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Sorting time for single process: " << duration.count() << " seconds\n";
    return 0;
}